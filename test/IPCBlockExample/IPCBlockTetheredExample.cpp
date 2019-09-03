//
// Created by wuyuanyi on 2019-08-19.
//

#include <boost/process.hpp>

#include "ipc/ipc_block.h"
#include "ipc/ipc_port.h"

using namespace boost;

class IPCBlockTetheredExample : public IPCBlock {
private:
    process::child subprocess;
public:
    explicit IPCBlockTetheredExample(SimStruct &s) : IPCBlock(s) {
        int dimIn[] = {1};
        int dimOut[] = {1};
        registerInputPort(SS_DOUBLE, 1, dimIn, COMPLEX_NO, FRAME_NO, false);
        registerOutputPort(SS_DOUBLE, 1, dimOut, COMPLEX_NO, false);

        SampleTime st{};
        st.sampleTime = 1;
        st.offset = 0;

        sampleTime.push_back(st);
    }

protected:
    void configureConnection() override {
        allowUntethered = false;
        port = 9990;
        connectionTimeout = std::chrono::seconds(2);
    }

public:
    void onStart() override {
        subprocess = process::child(process::search_path("python"), "test/IPCBlockExample/client.py");
        IPCBlock::onStart();    // this function will be blocking when allowUntethered is false.
    }

    void onTerminate() override {
        IPCBlock::onTerminate();
        if (subprocess.running()) {
            subprocess.terminate();
        }
    }
};

std::shared_ptr<Block> createBlock(SimStruct *S) {
    return std::make_shared<IPCBlockTetheredExample>(*S);
}