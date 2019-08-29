//
// Created by wuyuanyi on 2019-08-19.
//

#include "ipc_block.h"
#include "debug_utils.h"
#include "block_exceptions.h"
#include "message.h"
#include "msgpack.hpp"

IPCBlock::IPCBlock(SimStruct &s) :
    Block(s) {}
IPCBlock::~IPCBlock() = default;

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
void IPCBlock::acceptHandler(const boost::system::error_code &error, tcp::socket peer) {
    // re-accept
    acceptor->async_accept(
        ioService,
        std::bind(&IPCBlock::acceptHandler, this, std::placeholders::_1, std::placeholders::_2)
    );

    if (error) {
        throw RuntimeError(error.message());
    }
    int idx = clients.size();

    // Notify current state
    std::stringstream statusBuffer;
    StateMessage::makeStateNotifyPacket(statusBuffer, currentState);
    boost::asio::post(delegateService, [](){DEBUG_PRINTF("Connection accepted\n");});
    try {
        peer.send(buffer(statusBuffer.str()));
        clients.push_back(std::move(peer));
        boost::asio::post(delegateService, [](){DEBUG_PRINTF("New client connected\n");});
    } catch (std::exception &e) {
        peer.close();
        boost::asio::post(delegateService, [](){DEBUG_PRINTF("Async send failed, client not added\n");});
    }
}

void IPCBlock::onTerminate() {
    NotifyStateChanged(STOPPED);
    if (!ioService.stopped()) {
        ioService.stop();
    }
    backgroundThread.join();
    for (auto& client: clients) {
        client.close();
    }
}
void IPCBlock::onUpdate() {
    NotifyPortUpdated();
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
    std::stringstream buf;
    StateMessage::makeStateNotifyPacket(buf, state);
    broadcastMessage(buf.str());
}
void IPCBlock::NotifyParameterChanged() {
    ParameterMessage msg;
    for (auto &param: dialogParameters) {
        msg.params.emplace_back(param->toString());
    }
    std::stringstream buf;
    msgpack::pack(buf, msg);
    broadcastMessage(buf.str());

}
void IPCBlock::NotifyPortUpdated() {
    PortUpdateMessage msg;
    for (auto &inport: inputPorts) {
        auto it = std::find(disabledIPCInputPorts.begin(), disabledIPCInputPorts.end(), inport.portId);
        if (it != disabledIPCInputPorts.end()) {
            // disabled input port
            continue;
        }
        PortMessageData data;
        data.dims = inport.dimension;
        data.typeId = inport.dataTypeId;
        data.dataRef.ptr = static_cast<const char *>(inport.portData.data);
        data.dataRef.size = inport.portData.size;
        msg.ports.emplace_back(data);
    }

    std::stringstream buf;
    msgpack::pack(buf, msg);
    broadcastMessage(buf.str());
}
void IPCBlock::broadcastMessage(std::string str) {
    std::shared_ptr<std::string> s = std::make_shared<std::string>(str);
    for (int i = 0; i < clients.size(); i++) {
        auto &peer = clients[i];
        peer.async_send(
            buffer(*s),
            [&, s, i](const boost::system::error_code &ec, std::size_t bytes_transferred) {
              if (ec) {
                  clients[i].close();
                  clients.erase(clients.begin() + i);
                  boost::asio::post(delegateService, [](){DEBUG_PRINTF("Async send failed, remove client\n");});
              }
            });
    }
}





