//
// Created by wuyuanyi on 2019-08-19.
//

#include "ipc_block.h"

#include <utility>
#include <string>
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
    backgroundThread = std::thread([&] {
      while (true) {
          try {
              ioService.run();
              break;
          } catch (std::exception &ex) {
              delegatePrint(std::string("ioService exception: ") + ex.what() + "\n");
          }
      }
    });
    ssPrintf("Listening on :%d\n", port);

    if (!allowUntethered) {
        std::mutex waitClientMutex;
        std::unique_lock<std::mutex> lck(waitClientMutex);
        auto waitResult = initialAcceptanceSemaphore.wait_for(lck, connectionTimeout);
        if (waitResult == std::cv_status::timeout) {
            stopRequest("Time out when wait for client connection");
        }
    }

    // Notify started
    NotifyStateChanged(STARTED);

}

void IPCBlock::onTerminate() {
    if (acceptor && acceptor->is_open()) {
        NotifyStateChanged(STOPPED);
        // if closing io service, the outstanding task will not be executed and resource will be leaked.
        for (auto &client: clients) {
            try {
                closeSocket(client);
            } catch (std::exception &ex) {
                // ignore exception when closing client.
            }
        }
        acceptor->close();
    }
    if (backgroundThread.joinable()) {
        backgroundThread.join();
    }
    Block::onTerminate();
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
            data.dataRef = localInputPort->portData.toString();
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
        auto &sr = clients[i];
        sendMessage(str, sr);
    }
}

void IPCBlock::sendMessage(const std::string &str, std::shared_ptr<SocketResources> sr) {
    std::shared_ptr<std::string> s = std::make_shared<std::string>(str);
    sr->socket->async_send(
        buffer(*s),
        // shared_ptr should be passed as value; The reference is invalid in the lambda.
        [&, s, sr](const boost::system::error_code &ec, std::size_t bytes_transferred) {
          if (ec) {
              delegateDebugPrint("Send handler error: " + ec.message() + "\n");
              closeSocket(sr);
          }
        });
}

void IPCBlock::acceptHandler(const boost::system::error_code &error, tcp::socket peer) {
    if (error) {
        delegateDebugPrint("Accept handler error: " + error.message() + "\n");
        return;
    }

    // re-accept
    acceptor->async_accept(
        ioService,
        std::bind(&IPCBlock::acceptHandler, this, std::placeholders::_1, std::placeholders::_2)
    );

    // Notify current state
    std::stringstream buf;
    delegateDebugPrint("Connection accepted\n");

    NotifyStateChanged(currentState);
    try {
        const std::shared_ptr<SocketResources> &sr = std::make_shared<SocketResources>(peer,
                                                                                       receiverBufferSize);
        clients.push_back(sr);
        attachAsyncReceiver(sr);
        delegateDebugPrint("New client connected\n");

        // allow the tethered instance continue when client is availble.
        initialAcceptanceSemaphore.notify_one();

    } catch (std::exception &e) {
        peer.close();
        delegateDebugPrint("Async send failed, client not added\n");
    }
}

void IPCBlock::attachAsyncReceiver(std::shared_ptr<SocketResources> sr) {
    sr->socket->async_read_some(
        buffer(sr->receiveBuffer.get(), receiverBufferSize),
        [&, sr](const boost::system::error_code &ec, std::size_t bytes_transferred) {
          if (ec) {
              delegateDebugPrint("Receive handler error: " + ec.message() + "\n");
              closeSocket(sr);
              return;
          }
          streamUnpacker.reserve_buffer(bytes_transferred);
          memcpy(streamUnpacker.buffer(), sr->receiveBuffer.get(), bytes_transferred);
          streamUnpacker.buffer_consumed(bytes_transferred);

          msgpack::object_handle result;
          while (streamUnpacker.next(result)) {
              processIncomingMessage(result, sr);
          }

          attachAsyncReceiver(sr);
        }
    );
}

void IPCBlock::closeSocket(std::shared_ptr<SocketResources> sr) {
    if (!sr) {
        // peer has been removed. This function was called due to async execution. Ignore.
        return;
    }

    if (sr->socket->is_open()) {
        boost::system::error_code ec;
        sr->socket->shutdown(socket_base::shutdown_type::shutdown_both, ec);
        sr->socket->close();
    }

    auto found = std::find(clients.begin(), clients.end(), sr);
    if (found == clients.end()) {
        // Already removed
    } else {
        clients.erase(found);
        delegateDebugPrint("socket closed, client removed\n");

        if (!allowUntethered && clients.empty()) {
            delegateStopRequest("The last client disconnects.\n");
        }
    }
}

void IPCBlock::processIncomingMessage(msgpack::object_handle &oh, std::shared_ptr<SocketResources> sr) {
    const msgpack::object &object = oh.get();
    try {
        std::lock_guard<std::mutex> lock(mainThreadAccessMutex);

        std::map<std::string, msgpack::object> map = object.as<std::map<std::string, msgpack::object>>();
        msgpack::object &t = map["t"];
        msgpack::object d;
        std::vector<msgpack::object> dataVector;
        int messageType = t.as<int>();
        switch (messageType) {
            case ipc::STATUS:throw std::runtime_error("STATUS not supported");
            case ipc::PARAMS:throw std::runtime_error("PARAMS not supported");
            case ipc::PORTS:
                // retrieve data object
                d = map["d"];
                dataVector = d.as<std::vector<msgpack::object>>();

                // do not race with main thread
                for (msgpack::object &dataObject : dataVector) {
                    ipc::PortData p;
                    p = dataObject.as<ipc::PortData>();

                    if (p.id >= outputPorts.size()) {
                        throw std::runtime_error("Port id out of range");
                    }
                    std::shared_ptr<OutputPort> op = outputPorts[p.id];
                    const std::shared_ptr<IPCOutputPort> &localOP = std::dynamic_pointer_cast<IPCOutputPort>(
                        outputPorts[p.id]);
                    DEBUG_ASSERT(localOP, "Failed to cast IPCOutputPort");

                    if (localOP->allowRemote) {
                        op->portData.copyFrom(p.dataRef.c_str(), p.dataRef.size());
                    } else {
                        throw std::runtime_error("Output port " + std::to_string(p.id) + " is not writable");
                    }
                }
                break;
            case ipc::ERR:d = map["d"];
                // error is string
                throw std::runtime_error(d.as<std::string>());
            case ipc::INFO:NotifyBlockInfo();
                break;
            default:throw msgpack::type_error();
        }

    } catch (std::exception &e) {
        delegateDebugPrint(std::string(e.what()) + "\n");
        std::stringstream ss;
        std::string what = e.what();
        makeOutboundMessage(ipc::ERR, what, ss);
        sendMessage(ss.str(), sr);
    }
}

std::shared_ptr<InputPort> IPCBlock::buildInputPort(std::shared_ptr<InputPort> ref) {
    return std::make_shared<IPCInputPort>(*ref);
}

std::shared_ptr<OutputPort> IPCBlock::buildOutputPort(std::shared_ptr<OutputPort> ref) {
    return std::make_shared<IPCOutputPort>(*ref);
}








