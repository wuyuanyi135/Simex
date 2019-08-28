//
// Created by wuyuanyi on 2019-08-19.
//

#include <cpp_api/ipc/ipc_block.h>
class IPCBlockExample: public IPCBlock {
 public:
  IPCBlockExample(SimStruct &s) : IPCBlock(s) {
      int dim[] = {100};
      registerInputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO, false);

      SampleTime st;
      st.sampleTime = 1;
      st.offset = 0;

      sampleTime.push_back(st);
  }
 protected:
  void configureConnection() override {
    allowUntethered = true;
    port = 9990;
    connectionTimeoutSeconds = 2;
  }
};
std::shared_ptr<Block> createBlock(SimStruct *S) {
    return std::make_shared<IPCBlockExample>(*S);
}