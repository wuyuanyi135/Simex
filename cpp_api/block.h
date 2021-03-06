//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BLOCK_H
#define SIMEX_BLOCK_H

#include <boost/asio/io_service.hpp>
#include <vector>
#include <memory>
#include "port.h"
#include "dialog_parameter.h"
#include "sample_time.h"

#include "simulink_headers.h"

class Block {
public:
    // Whether it has been started and the runtime object is created;
    bool isStarted{false};

public:
    SimStruct *simStructReference;

public:
    std::vector<std::shared_ptr<InputPort>> inputPorts;
    std::vector<std::shared_ptr<OutputPort>> outputPorts;
    int numContinuousStates{0};
    int numDiscreteStates{0};
    std::vector<std::shared_ptr<DialogParameterBase>> dialogParameters;
    int numRWorks{0};
    int numIWorks{0};
    int numPWorks{0};
    int numModes{0};
    int numNonsampledZeroCrossings{0};
    unsigned int options{0};
    bool allowSignalsWithMoreThan2D{false};
    std::vector<SampleTime> sampleTime;

    virtual std::shared_ptr<InputPort> registerInputPort(int dataTypeId,
                                                         int numDimension,
                                                         int *dimensions,
                                                         int complexity,
                                                         int acceptFrameData,
                                                         int directFeedthrough);

    virtual std::shared_ptr<OutputPort> registerOutputPort(int dataTypeId,
                                                           int numDimension,
                                                           int *dimensions,
                                                           int complexity,
                                                           int acceptFrameData);

    virtual std::shared_ptr<InputPort> registerVariableSizedInputPort(int dataTypeId,
                                                                      int complexity,
                                                                      int acceptFrameData,
                                                                      int directFeedthrough);

    virtual std::shared_ptr<OutputPort>
    registerVariableSizedOutputPort(int dataTypeId, int complexity, int acceptFrameData);

    template<typename T>
    void registerBlockParameter(bool tunable) {
        this->dialogParameters.push_back(std::make_shared<DialogParameter<T>>(this->dialogParameters.size(), tunable));
    }

    explicit Block(SimStruct &S);

    virtual ~Block();

public:
    virtual void onInitialParameterProcessed() {
        // this callback is invoked when the first dialog parameters been processed.
        // If the block configuration depends on the parameter, initialize them here.
    }

    virtual void onStart() {
        isStarted = true;
    };

    // Do not assume any stage has been executed properly.
    virtual void onTerminate() {
        isStarted = false;
        if (!delegateService.stopped()) {
            delegateService.stop();
        }
    };

    virtual void onUpdate() {
        delegateService.poll();
    };

    virtual void onOutput() {};

    virtual void onParameterUpdated() {}

protected:
    // used for invoke MATLAB/Simulink calls on the main thread. Do not use infinite loop here
    boost::asio::io_service delegateService;
    boost::asio::io_service::work work{delegateService};

protected:
    // override these function when the port type should be override
    virtual std::shared_ptr<InputPort> buildInputPort(std::shared_ptr<InputPort> ref);

    virtual std::shared_ptr<OutputPort> buildOutputPort(std::shared_ptr<OutputPort> ref);

public:
    std::mutex mainThreadAccessMutex;

protected:
    // single-threading utilities
    virtual void stopRequest(std::string msg);

protected:
    // multi-threading utilities
    virtual void delegatePrint(const std::string &msg);

    virtual void delegateDebugPrint(const std::string &msg);

    virtual void delegateStopRequest(const std::string &msg);
};

#endif //SIMEX_BLOCK_H
