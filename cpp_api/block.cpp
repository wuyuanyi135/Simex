//
// Created by wuyua on 2019-07-09.
//

#include "block.h"
#include "port.h"
#include "block_lifecycle.h"

Block::Block(SimStruct& S): _simulinkBlockReference(&S), _blockMetaReference(createBlockMeta(&S)) {
    for (int i = 0; i < _blockMetaReference->inputPorts.size(); i++) {
        BLOCK_REGISTER_INPUT_PORT(this, i);
    }
    for (int i = 0; i < _blockMetaReference->outputPorts.size(); i++) {
        BLOCK_REGISTER_OUTPUT_PORT(this, i);
    }
}