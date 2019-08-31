//
// Created by wuyuanyi on 2019-08-19.
//

#include "ipc_block.h"

#include <utility>
#include "debug_utils.h"
#include "block_exceptions.h"
#include "message.h"
#include "ipc_port.h"

IPCBlock::IPCBlock(SimStruct &s) :
        Block(s) {}

IPCBlock::~IPCBlock() {

};

void IPCBlock::onStart() {
    configureConnection();
    tcp::endpoint endPoint{tcp::v4(), port};
    acceptor = std::make_unique<tcp::acceptor>(ioService, endPoint);
    acceptor->listen();
    acceptor->async_accept(
            ioService,
            std::bind(&IPCBlock::acceptHandler, this, std::placeholders::_1, std::placeholders::_2)
    );
    backgroundThread = std::thread([&] { ioService.run(); });
    DEBUG_PRINTF("Listening on :%d\n", port);

// Notify started
    NotifyStateChanged(STARTED);

}


void IPCBlock::onTerminate() {
    NotifyStateChanged(STOPPED);

    if (acceptor->is_open()) {
        // if closing io service, the outstanding task will not be executed and resource will be leaked.
        for (auto &client: clients) {
            closeSocket(client);
        }
        acceptor->close();
    }

    backgroundThread.join();

}

void IPCBlock::onUpdate() {
    NotifyInputPortUpdated();
    Block::onUpdate();
}

void IPCBlock::onOutput() {}

void IPCBlock::onParameterUpdated() {
    if (isStarted) {
        NotifyParameterChanged();
    }
}

void IPCBlock::NotifyStateChanged(BlockState state) {
    currentState = state;
    int _state = state;
    std::stringstream buf;
    makeOutboundMessage(ipc::STATUS, _state, buf);
    broadcastMessage(buf.str());
}

void IPCBlock::NotifyParameterChanged() {
    std::vector<std::string> msg;
    for (auto &param: dialogParameters) {
        msg.emplace_back(param->toString());
    }
    std::stringstream buf;
    makeOutboundMessage(ipc::PARAMS, msg, buf);
    broadcastMessage(buf.str());
}

void IPCBlock::NotifyInputPortUpdated() {
    std::vector<ipc::PortData> msg;
    for (const std::shared_ptr<InputPort> &inport: inputPorts) {
        const std::shared_ptr<IPCInputPort> &localInputPort = std::dynamic_pointer_cast<IPCInputPort>(inport);
        DEBUG_ASSERT(localInputPort, "Failed to cast InputPort to IPCInputPort");

        if (localInputPort->allowRemote) {
            ipc::PortData data;
            data.id = localInputPort->portId;
            data.dataRef.ptr = localInputPort->portData.data.get();
            data.dataRef.size = localInputPort->portData.size;
            msg.emplace_back(data);
        }
    }

    std::stringstream buf;
    makeOutboundMessage(ipc::PORTS, msg, buf);
    broadcastMessage(buf.str());
}

void IPCBlock::NotifyBlockInfo() {
    ipc::BlockInfo info;
    std::vector<ipc::SampleTimeInfo> sts;
    for (SampleTime &st: sampleTime) {
        ipc::SampleTimeInfo sti{};
        sti.sampleTime = st.sampleTime;
        sti.offsetTime = st.offset;
        sts.emplace_back(sti);
    }
    info.sampleTimes = sts;

    std::vector<ipc::PortInfo> inPortInfo;
    for (const auto &ip : inputPorts) {
        ipc::PortInfo portInfo{};
        const std::shared_ptr<IPCInputPort> &localIP = std::dynamic_pointer_cast<IPCInputPort>(ip);
        DEBUG_ASSERT(localIP, "Failed to cast IPCInputPort");
        portInfo.portId = localIP->portId;
        portInfo.dims = localIP->dimension;
        portInfo.typeId = localIP->dataTypeId;
        portInfo.allowRemote = localIP->allowRemote;
        inPortInfo.emplace_back(portInfo);
    }
    info.inputPorts = inPortInfo;

    std::vector<ipc::PortInfo> outPortInfo;
    for (const auto &ip : outputPorts) {
        ipc::PortInfo portInfo{};
        const std::shared_ptr<IPCOutputPort> &localOP = std::dynamic_pointer_cast<IPCOutputPort>(ip);
        DEBUG_ASSERT(localOP, "Failed to cast IPCOutputPort");
        portInfo.portId = localOP->portId;
        portInfo.dims = localOP->dimension;
        portInfo.typeId = localOP->dataTypeId;
        portInfo.allowRemote = localOP->allowRemote;
        outPortInfo.emplace_back(portInfo);
    }
    info.outputPorts = outPortInfo;

    std::stringstream buf;
    makeOutboundMessage(ipc::INFO, info, buf);
    broadcastMessage(buf.str());
}

void IPCBlock::broadcastMessage(const std::string &str) {
    std::shared_ptr<std::string> s = std::make_shared<std::string>(str);
    for (int i = 0; i < clients.size(); i++) {
        auto &peer = clients[i];
        sendMessage(str, peer);
    }
}

void IPCBlock::sendMessage(const std::string &str, std::shared_ptr<tcp::socket> peer) {
    std::shared_ptr<std::string> s = std::make_shared<std::string>(str);
    peer->async_send(
            buffer(*s),
            // shared_ptr should be passed as value; The reference is invalid in the lambda.
            [&, s, peer](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                if (ec) {
                    delegateService.post([&]() { DEBUG_PRINTF("Send handler error: %s\n", ec.message().c_str()); });
                    closeSocket(peer);
                }
            });
}

void IPCBlock::acceptHandler(const boost::system::error_code &error, tcp::socket peer) {
    if (error) {
        delegateService.post([&]() { DEBUG_PRINTF("Accpet handler error: %\n", error.message().c_str()); });
        return;
    }

    // re-accept
    acceptor->async_accept(
            ioService,
            std::bind(&IPCBlock::acceptHandler, this, std::placeholders::_1, std::placeholders::_2)
    );

    // Notify current state
    std::stringstream buf;

    boost::asio::post(delegateService, []() { DEBUG_PRINTF("Connection accepted\n"); });

    NotifyStateChanged(currentState);
    try {
        const std::shared_ptr<tcp::socket> &val = std::make_shared<tcp::socket>(std::move(peer));
        clients.push_back(val);
        attachAsyncReceiver(val);

        boost::asio::post(delegateService, []() { DEBUG_PRINTF("New client connected\n"); });
    } catch (std::exception &e) {
        peer.close();
        boost::asio::post(delegateService, []() { DEBUG_PRINTF("Async send failed, client not added\n"); });
    }
}

void IPCBlock::attachAsyncReceiver(const std::shared_ptr<tcp::socket> &peer) {
    char *allocatedBuffer = static_cast<char *>(socketReceiverPool.ordered_malloc(receiverBufferSize));
    // Keep the buffer in memory
    std::shared_ptr<char[]> keep(allocatedBuffer, [&](char *p) { socketReceiverPool.ordered_free(p); });
    // Create a new receiving buffer
    peer->async_read_some(
            buffer(allocatedBuffer, receiverBufferSize),
            [this, keep, peer](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                if (ec) {
                    delegateService.post([&]() { DEBUG_PRINTF("Receive handler error: %s\n", ec.message().c_str()); });
                    closeSocket(peer);
                    return;
                }
                streamUnpacker.reserve_buffer(bytes_transferred);
                memcpy(streamUnpacker.buffer(), keep.get(), bytes_transferred);
                streamUnpacker.buffer_consumed(bytes_transferred);

                msgpack::object_handle result;
                while (streamUnpacker.next(result)) {
                    processIncomingMessage(result, peer);
                }

                attachAsyncReceiver(peer);
            }
    );
}

void IPCBlock::closeSocket(const std::shared_ptr<tcp::socket> &peer) {
    if (!peer) {
        // peer has been removed. This function was called due to async execution. Ignore.
        return;
    }
    if (peer->is_open()) {
        peer->shutdown(socket_base::shutdown_type::shutdown_both);
        peer->close();
    }
    auto found = std::find(clients.begin(), clients.end(), peer);
    if (found == clients.end()) {
        // Already removed
    } else {
        clients.erase(found);
        boost::asio::post(delegateService,
                          []() { DEBUG_PRINTF("Async send failed, remove client\n"); });
    }
}

template<typename T, typename Stream>
void IPCBlock::makeOutboundMessage(ipc::MessageType type, T &payload, Stream &s) {
    msgpack::packer<std::stringstream> packer(s);
    packer.pack_map(2);
    packer.pack("t");
    // type cast otherwise template compilation failed.
    packer.pack(static_cast<int>(type));

    packer.pack("d");
    packer.pack(payload);
}

void IPCBlock::processIncomingMessage(msgpack::object_handle &oh, std::shared_ptr<tcp::socket> peer) {
    const msgpack::object &object = oh.get();
    try {
        std::map<std::string, msgpack::object> map = object.as<std::map<std::string, msgpack::object>>();
        msgpack::object &t = map["t"];
        msgpack::object d;
        std::vector<ipc::PortData> portData;

        int messageType = t.as<int>();
        switch (messageType) {
            case ipc::STATUS:
                throw RuntimeError("STATUS not supported");
            case ipc::PARAMS:
                throw RuntimeError("PARAMS not supported");
            case ipc::PORTS:
                // retrieve data object
                d = map["d"];

                portData = d.as<std::vector<ipc::PortData>>();
                for (ipc::PortData &p : portData) {
                    if (p.id >= outputPorts.size()) {
                        throw RuntimeError("Port id out of range");
                    }
                    std::shared_ptr<OutputPort> op = outputPorts[p.id];
                    const std::shared_ptr<IPCOutputPort> &localOP = std::dynamic_pointer_cast<IPCOutputPort>(
                            outputPorts[p.id]);
                    DEBUG_ASSERT(localOP, "Failed to cast IPCOutputPort");

                    if (localOP->allowRemote) {
                        op->portData.reallocate(p.dataRef.size);
                        memcpy(op->portData.data.get(), p.dataRef.ptr, p.dataRef.size);
                    } else {
                        throw RuntimeError("Output port " + std::to_string(p.id) + " is not writable");
                    }
                }
                break;
            case ipc::ERR:
                d = map["d"];
                // error is string
                throw RuntimeError(d.as<std::string>());
            case ipc::INFO:
                NotifyBlockInfo();
                break;
            default:
                throw msgpack::type_error();
        }

    } catch (msgpack::type_error &e) {
        // packet is invalid
        throw RuntimeError("Packet type mismatch");
    } catch (RuntimeError &e) {
#ifdef DEBUG
        delegateService.dispatch([&e]() { DEBUG_PRINTF(e.what()); });
#endif
        std::stringstream ss;
        std::string what = e.what();
        makeOutboundMessage(ipc::ERR, what, ss);
        sendMessage(ss.str(), std::move(peer));
    }
}

std::shared_ptr<InputPort> IPCBlock::buildInputPort(std::shared_ptr<InputPort> ref) {
    return std::make_shared<IPCInputPort>(*ref);
}

std::shared_ptr<OutputPort> IPCBlock::buildOutputPort(std::shared_ptr<OutputPort> ref) {
    return std::make_shared<IPCOutputPort>(*ref);
}








