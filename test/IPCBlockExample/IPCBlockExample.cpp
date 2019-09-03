//
// Created by wuyuanyi on 2019-08-19.
//

#include "ipc/ipc_block.h"
#include "ipc/ipc_port.h"

class IPCBlockExample : public IPCBlock {
public:
    explicit IPCBlockExample(SimStruct &s) : IPCBlock(s) {
        int dimIn[] = {2};
        int dimOut[] = {1};
        registerInputPort(SS_DOUBLE, 1, dimIn, COMPLEX_NO, FRAME_NO, false);
        registerOutputPort(SS_DOUBLE, 1, dimOut, COMPLEX_NO, false);
        const std::shared_ptr<OutputPort> &noRemote = registerOutputPort(SS_DOUBLE, 1, dimOut, COMPLEX_NO, false);
        std::dynamic_pointer_cast<IPCOutputPort>(noRemote)->allowRemote = false;

        SampleTime st{};
        st.sampleTime = 1;
        st.offset = 0;

        sampleTime.push_back(st);
    }

protected:
    void configureConnection() override {
        allowUntethered = true;
        port = 9990;
        connectionTimeout = std::chrono::seconds(2);
    }
};

std::shared_ptr<Block> createBlock(SimStruct *S) {
    return std::make_shared<IPCBlockExample>(*S);
}