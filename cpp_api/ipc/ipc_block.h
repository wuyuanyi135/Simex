//
// Created by wuyuanyi on 2019-08-19.
//
// Extension block that support IPC for customized user interface or external processing.
// A server will be started on the given port and ip. The default mode is untethered, where the client does not have to be connected.
// Otherwise, after the waiting time out,the block will fail on the initialization stage.

#ifndef SIMEX_IPCBLOCK_H
#define SIMEX_IPCBLOCK_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/pool/pool.hpp>

#include <thread>
#include <string>

#include "block_state.h"
#include "block.h"
#include "msgpack.hpp"
#include "message.h"

using namespace boost::asio;
using namespace boost::asio::ip;
using Pool = boost::pool<boost::default_user_allocator_malloc_free>;

class IPCBlock : public Block {
public:
    explicit IPCBlock(SimStruct &s);

    ~IPCBlock() override;

    void onStart() override;

    void onTerminate() override;

    void onUpdate() override;

    void onOutput() override;

    void onParameterUpdated() override;

protected:
    // configure the server ip, port, and the behavior if the server is not available.
    virtual void configureConnection() = 0;

protected:
    // server configuration
    bool allowUntethered = true;
    unsigned short port = 9090;
    int connectionTimeoutSeconds = 10;
    unsigned long long receiverBufferSize = 10240;

protected:
    // notify the clients about state or data change
    BlockState currentState = STOPPED;

    void NotifyStateChanged(BlockState state);

    void NotifyParameterChanged();

    void NotifyInputPortUpdated();

    void NotifyBlockInfo();
protected:
    // networking members
    io_service ioService;
    std::unique_ptr<tcp::acceptor> acceptor; // will be initialized later
    std::vector<std::shared_ptr<tcp::socket>> clients;
    std::thread backgroundThread;

    Pool socketReceiverPool{receiverBufferSize, receiverBufferSize};
    msgpack::unpacker streamUnpacker{};

protected:
    // callback handler for new connection
    void acceptHandler(const boost::system::error_code &error, tcp::socket peer);

    void broadcastMessage(const std::string& str);
    void sendMessage(const std::string& str, std::shared_ptr<tcp::socket> peer);

    void attachAsyncReceiver(const std::shared_ptr<tcp::socket>& peer);

    // remove the peer from registered clienconst ts
    void closeSocket(const std::shared_ptr<tcp::socket>&);

protected:
    template <typename T, typename Stream>
    void makeOutboundMessage(ipc::MessageType type, T& payload, Stream& s);
    void processIncomingMessage(msgpack::object_handle& oh, std::shared_ptr<tcp::socket> peer);

    std::shared_ptr<InputPort> buildInputPort(std::shared_ptr<InputPort> ref) override;

    std::shared_ptr<OutputPort> buildOutputPort(std::shared_ptr<OutputPort> ref) override;
};

#endif //SIMEX_IPCBLOCK_H
