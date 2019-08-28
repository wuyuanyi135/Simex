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
#include <thread>

#include "block_state.h"
#include "block.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class IPCBlock : public Block {
 public:
  explicit IPCBlock(SimStruct &s);
  ~IPCBlock() override;
  void onStart() override;
  void onTerminate() override;
  void onUpdate() override;
  void onOutput() override;
 protected:
  // configure the server ip, port, and the behavior if the server is not available.
  virtual void configureConnection() = 0;
 protected:
  bool allowUntethered = true;
  unsigned short port = 9090;
  int connectionTimeoutSeconds = 10;

 protected:
  // grpc utils
  BlockState currentState = STOPPED;
  void NotifyStateChanged(BlockState state);
  void NotifyParameterChanged();
  void NotifyPortUpdated();


 public:
  void onParameterUpdated() override;

 protected:
  io_service ioService;
  std::unique_ptr<tcp::acceptor> acceptor; // will be initialized later
  std::vector<tcp::socket> clients;
  std::thread backgroundThread;

 protected:
  // callback handler for new connection
  void acceptHandler(const boost::system::error_code& error, tcp::socket peer);
  void broadcastMessage(const_buffer buf);

 protected:
  // store the index of the input ports that ipc won't send.
  std::vector<int> disabledIPCInputPorts;

};

#endif //SIMEX_IPCBLOCK_H
