//
// Created by wuyuanyi on 2019-08-27.
//
#define BOOST_TEST_MODULE IPC_BLOCK_TEST
#include <boost/test/unit_test.hpp>
#include <msgpack.hpp>
#include <ipc/message.h>
#include <cpp_api/block_state.h>
using namespace msgpack;
BOOST_AUTO_TEST_CASE(statusPacketTest) {
    std::stringstream sbuf;
    StateMessage::makeStateNotifyPacket(sbuf, STARTED);

    msgpack::object_handle oh = msgpack::unpack(sbuf.str().data(), sbuf.str().size());
    msgpack::object obj = oh.get();
    BaseMessage bm;
    obj.convert(bm);
    BOOST_TEST_MESSAGE("[BM] type=" << bm.type);

    StateMessage sm;
    obj.convert(sm);
    BOOST_TEST_MESSAGE("[SM] type=" << sm.type <<" state=" <<sm.state);
}