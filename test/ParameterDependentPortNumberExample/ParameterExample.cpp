//
// Created by wuyuanyi on 12/07/19.
//

#include "block.h"

class ParameterExampleBlock : public Block {
public:
    explicit ParameterExampleBlock(SimStruct &S) : Block(S) {
        registerBlockParameter<int32_T>(false); //num input
        registerBlockParameter<int32_T>(false); //num output
    }

    void onParameterUpdated() override {
        Block::onParameterUpdated();
        auto paramNumInports = std::dynamic_pointer_cast<DialogParameter<int32_T>>(dialogParameters[0]);
        auto paramNumOutports = std::dynamic_pointer_cast<DialogParameter<int32_T>>(dialogParameters[1]);

        int dim[] = {1};
        for (int i = 0; i < paramNumInports->data; i++) {
            registerInputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO, true);
        }
        for (int i = 0; i < paramNumOutports->data; i++) {
            registerOutputPort(SS_DOUBLE, 1, dim, COMPLEX_NO, FRAME_NO);
        }
    }

    void onUpdate() override {}

    void onOutput() override {}

    void onStart() override {}

    void onTerminate() override {}
};

std::shared_ptr<Block> createBlock(SimStruct *S) {
    return std::make_shared<ParameterExampleBlock>(*S);
}