//
// Created by wuyua on 2019-07-07.
//

#include <simulink_impl/datatype_size.h>
#include "port.h"
#include "block.h"
#include "dimension_to_width.h"
#include "debug_utils.h"
#include "utils.h"

RuntimeOutputPort::RuntimeOutputPort(Block &runtimeBlockRef, int portId) : RuntimePort(runtimeBlockRef, portId) {
    OutputPort &portMeta = runtimeBlockRef._blockMetaReference->outputPorts[portId];
    this->sampleTime = portMeta.sampleTime;
    this->dataTypeId = portMeta.dataTypeId;
    this->acceptFrameData = portMeta.acceptFrameData;
    this->complexity = portMeta.complexity;
    this->dimension = portMeta.dimension;

    this->length = dimensionToWidth(this->dimension) * dataTypeIdToByteSize(this->dataTypeId);
    this->data = new uint8_T[this->length]();
    DEBUGV_LIFECYCLE_PRINTF("RuntimeOutputPort: %d of block %d is created\n", portId,
                            BLOCK_PTR(_runtimeBlockRef->_simulinkBlockReference));
}

RuntimeOutputPort::~RuntimeOutputPort() {
    DEBUGV_LIFECYCLE_PRINTF("RuntimeOutputPort: %d of block %d is released\n", portId,
                            BLOCK_PTR(_runtimeBlockRef->_simulinkBlockReference));
}

RuntimeInputPort::RuntimeInputPort(Block &runtimeBlockRef, int portId) : RuntimePort(runtimeBlockRef, portId) {
    InputPort &portMeta = runtimeBlockRef._blockMetaReference->inputPorts[portId];
    this->sampleTime = portMeta.sampleTime;
    this->dataTypeId = portMeta.dataTypeId;
    this->acceptFrameData = portMeta.acceptFrameData;
    this->complexity = portMeta.complexity;
    this->dimension = portMeta.dimension;

    this->length = dimensionToWidth(this->dimension) * dataTypeIdToByteSize(this->dataTypeId);
    this->data = new uint8_T[this->length]();
    DEBUGV_LIFECYCLE_PRINTF("RuntimeInputPort: %d of block %d is created\n", portId,
                            BLOCK_PTR(_runtimeBlockRef->_simulinkBlockReference));
}

RuntimeInputPort::~RuntimeInputPort() {
    if (this->data != nullptr) delete[] static_cast<uint8_T *>(this->data);
    DEBUGV_LIFECYCLE_PRINTF("RuntimeInputPort: %d of block %d is released\n", portId,
                            BLOCK_PTR(_runtimeBlockRef->_simulinkBlockReference));
}
