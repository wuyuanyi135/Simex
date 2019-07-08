//
// Created by wuyua on 2019-07-08.
//


#include <simstruc.h>
#include "block.h"
#include "block_meta.h"
#include "debug_utils.h"

BlockMeta::BlockMeta() {

    DEBUGV_LIFECYCLE_PRINTF("BlockMeta Initialized (SimpleExample)\n");

    OutputPort op;

    op.dimension.push_back(1);
    op.dataTypeId = 0;
    op.complexity = COMPLEX_INHERITED;
    op.acceptFrameData = 0;
    outputPorts.push_back(op);

}

BlockMeta::~BlockMeta() {
    DEBUGV_LIFECYCLE_PRINTF("BlockMeta destroyed\n");
}
