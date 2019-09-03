#include <simstruc.h>
#include <cpp_api/block_lifecycle.h>
#include "block.h"
#include "debug_utils.h"

/// Block
class VariableSizeExample : public Block {
private:
    DynamicData dd{0};
public:
    VariableSizeExample(SimStruct &S) : Block(S) {
        DEBUGV_LIFECYCLE_PRINTF("Block Initialized (SimpleExample)\n");
        registerVariableSizedInputPort(SS_DOUBLE, COMPLEX_NO, FRAME_NO, true);
        registerVariableSizedOutputPort(SS_DOUBLE, COMPLEX_NO, FRAME_NO);

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

    }

    void onOutput() override {
        int numEl = inputPorts[0]->dimension[0];

        dd.copyFrom(inputPorts[0]->portData);
        auto dataView = dd.getDataView<real64_T>();
        for (auto item: *dataView) {
            (*item) = (*item) + 1.0;
        }
        outputPorts[0]->portData.copyFrom(dd);
    }
};

std::shared_ptr<Block> createBlock(SimStruct *S) {
    return std::make_shared<VariableSizeExample>(*S);
}