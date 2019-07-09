//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BLOCK_META_H
#define SIMEX_BLOCK_META_H

#include <vector>
#include "port.h"
#include "dialog_parameter.h"
#include "sample_time.h"
#include "simstruc.h"

class BlockMeta {
public:
    std::vector<InputPort> inputPorts;
    std::vector<OutputPort> outputPorts;
    int numContinuousStates{0};
    int numDiscreteStates{0};
    std::vector<DialogParameter> dialogParameters;
    int numRWorks {0};
    int numIWorks {0};
    int numPWorks {0};
    int numModes {0};
    int numNonsampledZeroCrossings {0};
    int options {0};

    std::vector<SampleTime> sampleTime;

    void registerInputPort(int dataTypeId, int dimension, int complexity, int acceptFrameData, int directFeedthrough);
    void registerInputPort(int dataTypeId, int dimension1, int dimension2, int complexity, int acceptFrameData, int directFeedthrough);
    void registerOutputPort(int dataTypeId, int dimension, int complexity, int acceptFrameData);
    void registerOutputPort(int dataTypeId, int dimension1, int dimension2, int complexity, int acceptFrameData);

    BlockMeta(SimStruct& S);
    ~BlockMeta();
};
#endif //SIMEX_BLOCK_META_H
