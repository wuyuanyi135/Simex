//
// Created by wuyuanyi on 2019-09-01.
//

#ifndef SIMEX_SOCKET_RESOURCES_H
#define SIMEX_SOCKET_RESOURCES_H
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;
class SocketResources {
public:
    std::shared_ptr<tcp::socket> socket;
    std::shared_ptr<char> receiveBuffer;

    SocketResources(tcp::socket &socket, int receiveBufferSize);
};


#endif //SIMEX_SOCKET_RESOURCES_H
