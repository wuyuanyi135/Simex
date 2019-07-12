//
// Created by wuyuanyi on 12/07/19.
//

#include "block.h"

class ParameterExampleBlock : public Block {
 public:
  explicit ParameterExampleBlock(SimStruct &S) : Block(S) {
      int dim[] = {1};
      registerInputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO, true);
      registerOutputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO);

      registerBlockParameter<real64_T>(true); //Gain
      registerBlockParameter<std::string>(false); //Output this on update
  }

  void onUpdate() override {
      ssPrintf("Arg: %s\n", std::dynamic_pointer_cast<DialogParameter<std::string>>(dialogParameters[1])->data.c_str());

  }

  void onOutput() override {
      // This port is configured as direct feedthrough. Its value will be updated prior to output. Otherwise, it is updated before update.
      OUTPUTPORT_TARGET(outputPorts[0], real64_T*) =
          INPUTPORT_TARGET(inputPorts[0], real64_T*) *
              std::dynamic_pointer_cast<DialogParameter<real64_T>>(dialogParameters[0])->data;
  }
  void onStart() override {

  }
  void onTerminate() override {

  }
};

std::shared_ptr<Block> createBlock(SimStruct *S) {
    return std::make_shared<ParameterExampleBlock>(*S);
}