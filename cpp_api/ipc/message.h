//
// Created by wuyuanyi on 2019-08-21.
//

#ifndef SIMEX_MESSAGE_H
#define SIMEX_MESSAGE_H

#include "msgpack.hpp"
#include "block_state.h"
namespace ipc {
    enum MessageType {
        STATUS = 0,
        PARAMS = 1,
        PORTS = 2,
        ERR = 3,
        INFO = 4,
    };

    struct PortData {
        msgpack::type::raw_ref dataRef;
        int id;
        MSGPACK_DEFINE_MAP(id, dataRef);
    };

    struct PortInfo {
        int typeId;
        std::vector<int> dims;
        int portId;
        bool allowRemote;

        MSGPACK_DEFINE_MAP(typeId, portId, dims, allowRemote);
    };
    struct SampleTimeInfo {
        double sampleTime;
        double offsetTime;
        MSGPACK_DEFINE_MAP(sampleTime, offsetTime);
    };
    struct BlockInfo {
        std::vector<SampleTimeInfo> sampleTimes;
        std::vector<PortInfo> inputPorts;
        std::vector<PortInfo> outputPorts;

        MSGPACK_DEFINE_MAP(sampleTimes, inputPorts, outputPorts);
    };
}

#endif //SIMEX_MESSAGE_H
