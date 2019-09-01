//
// Created by wuyua on 2019-08-29.
//

#include "ipc_port.h"

IPCOutputPort::IPCOutputPort(const OutputPort &ref) : OutputPort(ref) {

}

IPCOutputPort::IPCOutputPort(int portId, Block *blockRef, int dataTypeId) : OutputPort(portId, blockRef, dataTypeId) {}

IPCInputPort::IPCInputPort(const InputPort &ref) : InputPort(ref) {}

IPCInputPort::IPCInputPort(int portId, Block *blockRef, int dataTypeId) : InputPort(portId, blockRef, dataTypeId) {}
