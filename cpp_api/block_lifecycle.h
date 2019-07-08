//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BLOCK_CREATION_H
#define SIMEX_BLOCK_CREATION_H

#include <memory>
#include "block.h"
#include "block_meta.h"
#include "simstruc.h"
std::shared_ptr<Block> createBlock(SimStruct* S);
std::shared_ptr<BlockMeta> createBlockMeta(SimStruct* S);

#endif //SIMEX_BLOCK_CREATION_H
