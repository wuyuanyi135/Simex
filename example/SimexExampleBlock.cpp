//
// Created by wuyuanyi on 17/07/19.
//

#include "block.h"

class SimexExampleBlock : public Block {
 public:
  explicit SimexExampleBlock(SimStruct &S) : Block(S) {
      ssPrintf("Block created\n");
      int dim[] = {1};
      registerInputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO, true);
      registerOutputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO);
  }
  ~SimexExampleBlock() override {
      ssPrintf("Block destroyed\n");
  }
  void onInitializeRuntime() override {
      Block::onInitializeRuntime();
  }
  void onStart() override {

  }
  void onTerminate() override {

  }
  void onUpdate() override {

  }
  void onOutput() override {
    OUTPUTPORT_TARGET(outputPorts[0], double*) = INPUTPORT_TARGET(inputPorts[0], double*) * 2;
  }
};
