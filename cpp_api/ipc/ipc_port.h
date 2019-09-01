//
// Created by wuyua on 2019-08-29.
//

#ifndef SIMEX_IPC_PORT_H
#define SIMEX_IPC_PORT_H

#include "port.h"

class IPCInputPort: public InputPort {
public:
    bool allowRemote{true};

    explicit IPCInputPort(const InputPort &ref);

    IPCInputPort(int portId, Block *blockRef, int dataTypeId);
};
class IPCOutputPort: public OutputPort {
public:
    bool allowRemote{true};

    IPCOutputPort(int portId, Block *blockRef, int dataTypeId);

    explicit IPCOutputPort(const OutputPort& ref);
};

#endif //SIMEX_IPC_PORT_H
