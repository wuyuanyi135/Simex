//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BLOCK_H
#define SIMEX_BLOCK_H

#include <vector>
#include <memory>
#include "port.h"
#include "dialog_parameter.h"
#include "sample_time.h"
#include "simstruc.h"

class Block {
 public:
  // Whether it has been started and the runtime object is created;
  bool isStarted {false};

 public:
  SimStruct* simStructReference;

 public:
  std::vector<InputPort> inputPorts;
  std::vector<OutputPort> outputPorts;
  int numContinuousStates{0};
  int numDiscreteStates{0};
  std::vector<std::shared_ptr<DialogParameterBase>> dialogParameters;
  int numRWorks{0};
  int numIWorks{0};
  int numPWorks{0};
  int numModes{0};
  int numNonsampledZeroCrossings{0};
  int options{0};
  bool allowSignalsWithMoreThan2D{false};
  std::vector<SampleTime> sampleTime;

  void registerInputPort(int dataTypeId,
                         int numDimension,
                         int *dimensions,
                         int complexity,
                         int acceptFrameData,
                         int directFeedthrough);
  void registerOutputPort(int dataTypeId, int numDimension, int *dimensions, int complexity, int acceptFrameData);
  void registerVariableSizedInputPort(int dataTypeId,
                                      int complexity,
                                      int acceptFrameData,
                                      int directFeedthrough);
  void registerVariableSizedOutputPort(int dataTypeId, int complexity, int acceptFrameData);

  template <typename T>
  void registerBlockParameter(bool tunable) {
      this->dialogParameters.push_back(std::make_shared<DialogParameter<T>>(this->dialogParameters.size(), tunable));
  }

  explicit Block(SimStruct &S);
  ~Block();


 public:
  // Called before on start. Create memory for the port, etc.
  virtual void onInitializeRuntime();
  virtual void onStart() = 0;
  virtual void onTerminate() = 0;
  virtual void onUpdate() = 0;
  virtual void onOutput() = 0;
};


#endif //SIMEX_BLOCK_H
