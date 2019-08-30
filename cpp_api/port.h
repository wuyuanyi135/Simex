//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_PORT_H
#define SIMEX_PORT_H

#include <vector>
#include <simulink_impl/debug_utils.h>
#include "simulink_headers.h"

#include "sample_time.h"
#include "dynamic_data.h"


// Access the void* pointer of the output port
#define OUTPUTPORT_POINTER(OUTPUTPORT) (OUTPUTPORT->portData.data)

// Access the void* pointer of the input port
#define INPUTPORT_POINTER(INPUTPORT) (INPUTPORT->portData.data)

// Access the pointer with type cast short hand.
#define OUTPUTPORT_TYPED_POINTER(OUTPUTPORT, T) ((T)OUTPUTPORT_POINTER(OUTPUTPORT))

// Access the pointer with type cast short hand.
#define INPUTPORT_TYPED_POINTER(INPUTPORT, T) ((T)INPUTPORT_POINTER(INPUTPORT))

// Access the pointed memory (*(T)ptr) with type cast short hand.
#define OUTPUTPORT_TARGET(OUTPUTPORT, T) (* OUTPUTPORT_TYPED_POINTER(OUTPUTPORT, T))
#define INPUTPORT_TARGET(INPUTPORT, T) (* INPUTPORT_TYPED_POINTER(INPUTPORT, T))

class Block;

class Port {
 public:
  Port(int portId, Block *blockRef, int dataTypeId) : portId(portId), _blockRef(blockRef), dataTypeId(dataTypeId) {}
  int complexity{COMPLEX_NO};
  int dataTypeId{SS_DOUBLE};
  bool dynamicDimension{false};
  SampleTime sampleTime{};
  int acceptFrameData{FRAME_NO};
  std::vector<int> dimension;

  int portId;

 protected:
  // Only available during runtime:
  Block *_blockRef{nullptr};

 public:
  // Only available during runtime:
  DynamicData portData;

 public:
  // callbacks
  bool validateDimension() { return true; };
  bool validateDataType() { return true; };

 public:

};

class InputPort : public Port {
 public:
  int directFeedthrough{0};

 public:
  /// autoCopyFromSimulink == true: copy data from simulink in each Update callback.
  /// autoCopyFromSimulink == false: copy data from Simulink only when requestingUpdateFromSimulink is set. When dealing with large output
  /// this one should be used.
  bool autoCopyFromSimulink{true};
  bool requestingUpdateFromSimulink{false};
  InputPort(int portId, Block *blockRef, int dataTypeId) : Port(portId, blockRef, dataTypeId) {
      DEBUGV_LIFECYCLE_PRINTF("InputPort: %d is created\n",
                              portId);
  }
  virtual ~InputPort() { DEBUGV_LIFECYCLE_PRINTF("InputPort: %d is released\n", portId); }
};

class OutputPort : public Port {
 public:
  OutputPort(int portId, Block *blockRef, int dataTypeId) : Port(portId, blockRef, dataTypeId) {
      DEBUGV_LIFECYCLE_PRINTF("OutputPort: %d is created\n", portId);
  };
  /// autoCopyToSimulink == true: copy data to Simulink when data is different.
  /// autoCopyToSimulink == false: copy data to Simulink only when requestingUpdateToSimulink is set. When dealing with large output
  /// this one should be used.
  bool autoCopyToSimulink{true};
  bool requestingUpdateToSimulink{false};
  virtual ~OutputPort() {
      DEBUGV_LIFECYCLE_PRINTF("OutputPort: %d is released\n", portId);
  };
};

#endif //SIMEX_PORT_H
