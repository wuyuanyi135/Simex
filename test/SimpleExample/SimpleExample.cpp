//
// Created by wuyua on 2019-07-08.
//


#include <simstruc.h>
#include <cpp_api/block_lifecycle.h>
#include "block.h"
#include "debug_utils.h"

/// Block
class SimpleExampleBlock : public Block {
 public:
  SimpleExampleBlock(SimStruct &S) : Block(S) {
      DEBUGV_LIFECYCLE_PRINTF("Block Initialized (SimpleExample)\n");
      int dim[] = {1};
      registerInputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO, true);
      registerInputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO, true);
      registerOutputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO);

      SampleTime st;
      st.sampleTime = 1;
      st.offset = 0;
      sampleTime.push_back(st);
  }

  void onStart() override {
  }

  void onTerminate() override {
  }

  void onUpdate() override {
      // Expansion:         (* (real64_T*) (outputPorts[0]->data)) = *(real64_T *) inputPorts[0]->data + *(real64_T *) inputPorts[1]->data;
      OUTPUTPORT_TARGET(outputPorts[0], real64_T*) =
          INPUTPORT_TARGET(inputPorts[0], real64_T*) + INPUTPORT_TARGET(inputPorts[1], real64_T*);
  }

  void onOutput() override {
  }
};

std::shared_ptr<Block> createBlock(SimStruct *S) {
    return std::make_shared<SimpleExampleBlock>(*S);
}