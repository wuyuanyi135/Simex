//
// Created by wuyuanyi on 2019-09-01.
//

#include "socket_resources.h"

SocketResources::SocketResources(tcp::socket& socket, int receiveBufferSize) {
    this->socket = std::make_shared<tcp::socket>(std::move(socket));
    char *buf = new char[receiveBufferSize];
    receiveBuffer = std::shared_ptr<char>(buf, [](char *p) {
        delete[] p;
    });
}

