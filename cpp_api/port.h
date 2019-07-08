//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_PORT_H
#define SIMEX_PORT_H


#include <vector>
#include "sample_time.h"

class Port {
public:
    int complexity;
    int dataTypeId;

    SampleTime sampleTime;

    int acceptFrameData;

    std::vector<int> dimension;


};

class InputPort : public Port {
public:
    int directFeedthrough{};

};

class OutputPort : public Port {

};

#endif //SIMEX_PORT_H
