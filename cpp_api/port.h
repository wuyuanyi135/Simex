//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_PORT_H
#define SIMEX_PORT_H


#include <vector>
#include "sample_time.h"
#define BLOCK_REGISTER_INPUT_PORT(BLK, PORT_ID) BLK->inputPorts.push_back(std::make_shared<RuntimeInputPort>(*BLK, PORT_ID))
#define BLOCK_REGISTER_OUTPUT_PORT(BLK, PORT_ID) BLK->outputPorts.push_back(std::make_shared<RuntimeOutputPort>(*BLK, PORT_ID))

class Block;
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

// Dynamic port class. Synchronize the data with the Simulink block in each  cycle.
class RuntimePort: public Port {
public:
    RuntimePort(Block& runtimeBlockRef, int portId) : _runtimeBlockRef(&runtimeBlockRef), portId(portId) {}

protected:
    Block* _runtimeBlockRef{nullptr};

public:
    void* data{nullptr};
    int length {-1};

public:
    int portId {0};
};


class RuntimeInputPort: public RuntimePort {
public:
    RuntimeInputPort(Block &runtimeBlockRef, int portId);
    ~RuntimeInputPort();
public:
    /// autoCopyFromSimulink == true: copy data from simulink in each Update callback.
    /// autoCopyFromSimulink == false: copy data from Simulink only when requestingUpdateFromSimulink is set. When dealing with large output
    /// this one should be used.
    bool autoCopyFromSimulink {true};
    bool requestingUpdateFromSimulink {false};
};

class RuntimeOutputPort: public RuntimePort {
public:
    RuntimeOutputPort(Block &runtimeBlockRef, int portId);
    ~RuntimeOutputPort();
public:
    /// autoCopyToSimulink == true: copy data to Simulink when data is different.
    /// autoCopyToSimulink == false: copy data to Simulink only when requestingUpdateToSimulink is set. When dealing with large output
    /// this one should be used.
    bool autoCopyToSimulink {true};
    bool requestingUpdateToSimulink {false};
};
#endif //SIMEX_PORT_H
