//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BRIDGE_H
#define SIMEX_BRIDGE_H

#include "Simex.h"

#ifdef __cplusplus
extern "C" {
#endif
    int _initializeMeta(SimStruct* S);
    int _initialize (SimStruct* S);
    int _terminate (SimStruct* S);

#ifdef __cplusplus
}
#endif
#endif //SIMEX_BRIDGE_H
