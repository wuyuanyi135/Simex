//
// Created by wuyua on 2019-07-08.
//


#include <simstruc.h>
#include <cpp_api/block_lifecycle.h>
#include "block.h"
#include "block_meta.h"
#include "debug_utils.h"

BlockMeta::BlockMeta(SimStruct &S) {

  DEBUGV_LIFECYCLE_PRINTF("BlockMeta Initialized (SimpleExample)\n");

  registerInputPort(SS_DOUBLE, 1, COMPLEX_NO, FRAME_NO, true);
  registerInputPort(SS_DOUBLE, 1, COMPLEX_NO, FRAME_NO, true);
  registerOutputPort(SS_DOUBLE, 1, COMPLEX_NO, FRAME_NO);

  SampleTime st;
  st.sampleTime = 1;
  st.offset = 0;
  sampleTime.push_back(st);
}

BlockMeta::~BlockMeta() {
  DEBUGV_LIFECYCLE_PRINTF("BlockMeta destroyed\n");
}

/// Block
class SimpleExampleBlock : public Block {
 public:
  SimpleExampleBlock(SimStruct &S) : Block(S) {

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
std::shared_ptr<BlockMeta> createBlockMeta(SimStruct *S) {
  return std::make_shared<BlockMeta>(*S);
}