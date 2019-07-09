//
// Created by wuyua on 2019-07-09.
//
#include "block_meta.h"
void BlockMeta::registerInputPort(int dataTypeId, int dimension, int complexity, int acceptFrameData,
                                  int directFeedthrough) {
    InputPort ip = InputPort();
    ip.dimension.push_back(dimension);
    ip.dataTypeId = dataTypeId;
    ip.complexity = complexity;
    ip.acceptFrameData = acceptFrameData;
    ip.directFeedthrough = directFeedthrough;
    inputPorts.push_back(ip);
}

void BlockMeta::registerInputPort(int dataTypeId, int dimension1, int dimension2, int complexity, int acceptFrameData,
                                  int directFeedthrough) {
    InputPort ip = InputPort();
    ip.dimension.push_back(dimension1);
    ip.dimension.push_back(dimension2);
    ip.dataTypeId = dataTypeId;
    ip.complexity = complexity;
    ip.acceptFrameData = acceptFrameData;
    ip.directFeedthrough = directFeedthrough;
    inputPorts.push_back(ip);
}
void BlockMeta::registerOutputPort(int dataTypeId, int dimension, int complexity, int acceptFrameData) {
    OutputPort op = OutputPort();
    op.dimension.push_back(dimension);
    op.dataTypeId = dataTypeId;
    op.complexity = complexity;
    op.acceptFrameData = acceptFrameData;
    outputPorts.push_back(op);
}

void BlockMeta::registerOutputPort(int dataTypeId, int dimension1, int dimension2, int complexity, int acceptFrameData) {
    OutputPort op = OutputPort();
    op.dimension.push_back(dimension1);
    op.dimension.push_back(dimension2);
    op.dataTypeId = dataTypeId;
    op.complexity = complexity;
    op.acceptFrameData = acceptFrameData;
    outputPorts.push_back(op);
}