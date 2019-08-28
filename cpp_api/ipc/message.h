//
// Created by wuyuanyi on 2019-08-21.
//

#ifndef SIMEX_MESSAGE_H
#define SIMEX_MESSAGE_H
#include "msgpack.hpp"
#include "block_state.h"

class StateMessage {
 public:
  int state{};
  MSGPACK_DEFINE_MAP(state);
 public:
  static void makeStateNotifyPacket(std::stringstream& sbuf, BlockState state) {
      StateMessage msg;
      msg.state = state;
      msgpack::pack(sbuf, msg);
  }
};

class ParameterMessage {
 public:
  std::vector<std::string> params;
  MSGPACK_DEFINE_MAP(params)
};

class PortMessageData {
 public:
  std::vector<int> dims;
  int typeId;
  msgpack::type::raw_ref dataRef;
  MSGPACK_DEFINE_MAP(dims, typeId, dataRef);
};
class PortUpdateMessage {
 public:
  std::vector<PortMessageData> ports;
  MSGPACK_DEFINE_MAP(ports);
};
#endif //SIMEX_MESSAGE_H
