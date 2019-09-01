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

std::shared_ptr<InputPort> Block::registerInputPort(int dataTypeId,
                              int numDimension,
                              int *dimensions,
                              int complexity,
                              int acceptFrameData,
                              int directFeedthrough) {
    int id = inputPorts.size();
    std::shared_ptr<InputPort> ip = std::make_shared<InputPort>(id, this, dataTypeId);
    for (int i = 0; i < numDimension; ++i) {
        ip->dimension.push_back(dimensions[i]);
    }
    ip->dataTypeId = dataTypeId;
    ip->complexity = complexity;
    ip->acceptFrameData = acceptFrameData;
    ip->directFeedthrough = directFeedthrough;
    // use buildXXX adapter to provide customizable port type
    const std::shared_ptr<InputPort> transformed = buildInputPort(ip);
    inputPorts.push_back(transformed);
    return transformed;
}
std::shared_ptr<OutputPort> Block::registerOutputPort(int dataTypeId,
                               int numDimension,
                               int *dimensions,
                               int complexity,
                               int acceptFrameData) {
    int id = outputPorts.size();
    std::shared_ptr<OutputPort> op = std::make_shared<OutputPort>(id, this, dataTypeId);

    for (int i = 0; i < numDimension; ++i) {
        op->dimension.push_back(dimensions[i]);
    }
    op->dataTypeId = dataTypeId;
    op->complexity = complexity;
    op->acceptFrameData = acceptFrameData;
    const std::shared_ptr<OutputPort> transformed = buildOutputPort(op);
    outputPorts.push_back(transformed);
    return transformed;
}

std::shared_ptr<InputPort> Block::registerVariableSizedInputPort(int dataTypeId,
                                           int complexity,
                                           int acceptFrameData,
                                           int directFeedthrough) {
    int id = inputPorts.size();
    std::shared_ptr<InputPort> ip = std::make_shared<InputPort>(id, this, dataTypeId);

    ip->dynamicDimension = true;
    ip->dataTypeId = dataTypeId;
    ip->complexity = complexity;
    ip->acceptFrameData = acceptFrameData;
    ip->directFeedthrough = directFeedthrough;
    const std::shared_ptr<InputPort> transformed = buildInputPort(ip);
    inputPorts.push_back(transformed);
    return transformed;
}
std::shared_ptr<OutputPort> Block::registerVariableSizedOutputPort(int dataTypeId, int complexity, int acceptFrameData) {
    int id = outputPorts.size();
    std::shared_ptr<OutputPort> op = std::make_shared<OutputPort>(id, this, dataTypeId);
    op->dynamicDimension = true;
    op->dataTypeId = dataTypeId;
    op->complexity = complexity;
    op->acceptFrameData = acceptFrameData;
    const std::shared_ptr<OutputPort> transformed = buildOutputPort(op);
    outputPorts.push_back(transformed);
    return transformed;
}
void Block::onInitializeRuntime() {

    for (auto & p : inputPorts) {
        if (!p->portData.data) {
            int size = dimensionToWidth(p->dimension) * dataTypeIdToByteSize(p->dataTypeId);
            p->portData.reallocate(size);
        }
    }

    for (auto & p : outputPorts) {
        if (!p->portData.data) {
            int size = dimensionToWidth(p->dimension) * dataTypeIdToByteSize(p->dataTypeId);
            p->portData.reallocate(size);
        }
    }

}

std::shared_ptr<OutputPort> Block::buildOutputPort(std::shared_ptr<OutputPort> ref) {
    return ref;
}

std::shared_ptr<InputPort> Block::buildInputPort(std::shared_ptr<InputPort> ref) {
    return ref;
}
