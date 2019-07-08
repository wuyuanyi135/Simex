// Overridable functions for customized block creation and deletion
// Created by wuyua on 2019-07-07.
//

#include "block_lifecycle.h"

#ifdef USE_INTERNAL_BLOCK_LIFECYCLE
std::shared_ptr<Block> createBlock() {
    return std::make_shared<Block>();
}
std::shared_ptr<BlockMeta> createBlockMeta(SimStruct* S) {
    return std::make_shared<BlockMeta>();
}

#endif