//
// Created by wuyua on 2019-07-09.
//
#include <simulink_impl/debug_utils.h>
#include <simulink_impl/datatype_size.h>
#include "block.h"
#include "dimension_to_width.h"
Block::Block(SimStruct &S) : simStructReference(&S) {

}

Block::~Block() {
    DEBUGV_LIFECYCLE_PRINTF("Block destroyed\n");
}

void Block::registerInputPort(int dataTypeId,
                              int numDimension,
                              int *dimensions,
                              int complexity,
                              int acceptFrameData,
                              int directFeedthrough) {
    int id = inputPorts.size();
    InputPort ip = InputPort(id, this, dataTypeId);
    for (int i = 0; i < numDimension; ++i) {
        ip.dimension.push_back(dimensions[i]);
    }
    ip.dataTypeId = dataTypeId;
    ip.complexity = complexity;
    ip.acceptFrameData = acceptFrameData;
    ip.directFeedthrough = directFeedthrough;
    inputPorts.push_back(ip);
}
void Block::registerOutputPort(int dataTypeId,
                               int numDimension,
                               int *dimensions,
                               int complexity,
                               int acceptFrameData) {
    int id = outputPorts.size();
    OutputPort op = OutputPort(id, this, dataTypeId);
    for (int i = 0; i < numDimension; ++i) {
        op.dimension.push_back(dimensions[i]);
    }
    op.dataTypeId = dataTypeId;
    op.complexity = complexity;
    op.acceptFrameData = acceptFrameData;
    outputPorts.push_back(op);
}

void Block::registerVariableSizedInputPort(int dataTypeId,
                                           int complexity,
                                           int acceptFrameData,
                                           int directFeedthrough) {
    int id = inputPorts.size();
    InputPort ip = InputPort(id, this, dataTypeId);

    ip.dynamicDimension = true;
    ip.dataTypeId = dataTypeId;
    ip.complexity = complexity;
    ip.acceptFrameData = acceptFrameData;
    ip.directFeedthrough = directFeedthrough;
    inputPorts.push_back(ip);
}
void Block::registerVariableSizedOutputPort(int dataTypeId, int complexity, int acceptFrameData) {
    int id = outputPorts.size();
    OutputPort op = OutputPort(id, this, dataTypeId);
    op.dynamicDimension = true;
    op.dataTypeId = dataTypeId;
    op.complexity = complexity;
    op.acceptFrameData = acceptFrameData;
    outputPorts.push_back(op);
}
void Block::onInitializeRuntime() {

    for (auto & p : inputPorts) {
        if (!p.portData.data) {
            int size = dimensionToWidth(p.dimension) * dataTypeIdToByteSize(p.dataTypeId);
            p.portData.reallocate(size);
        }
    }

    for (auto & p : outputPorts) {
        if (!p.portData.data) {
            int size = dimensionToWidth(p.dimension) * dataTypeIdToByteSize(p.dataTypeId);
            p.portData.reallocate(size);
        }
    }

}
