//
// Created by wuyua on 2019-07-07.
//

#include <simulink_impl/datatype_size.h>
#include "port.h"
#include "dimension_to_width.h"
#include "debug_utils.h"
#include "utils.h"


Port::Port(int portId, Block *blockRef) : portId(portId), _blockRef(blockRef) {}

bool Port::validateDimension() {
    return true;
}

bool Port::validateDataType() {
    return true;
}

InputPort::InputPort(int portId, Block *blockRef) : Port(portId, blockRef) {
    DEBUGV_LIFECYCLE_PRINTF("RuntimeInputPort: %d is created\n", portId);
}

InputPort::~InputPort() {
    DEBUGV_LIFECYCLE_PRINTF("RuntimeInputPort: %d is released\n", portId);
}

OutputPort::OutputPort(int portId, Block *blockRef) : Port(portId, blockRef) {
    DEBUGV_LIFECYCLE_PRINTF("RuntimeOutputPort: %d is created\n", portId);
}

OutputPort::~OutputPort() {
    DEBUGV_LIFECYCLE_PRINTF("RuntimeOutputPort: %d is released\n", portId);
}
