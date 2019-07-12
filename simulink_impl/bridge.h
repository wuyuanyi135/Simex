//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BRIDGE_H
#define SIMEX_BRIDGE_H

#include "simstruc.h"
#ifdef __cplusplus
extern "C" {
#endif

void mdlCheckParameters(SimStruct *pTag);


int _initializeMeta(SimStruct *S);
int _initializeSampleTime(SimStruct *S);
int _start(SimStruct *S);
int _terminate(SimStruct *S);
int _output(SimStruct *S);
int _update(SimStruct *S);

int _setOutputPortDimensionInfo(SimStruct *S, int_T port, const DimsInfo_T *dims);
int _setInputPortDimensionInfo(SimStruct *S, int_T port, const DimsInfo_T *dims);

int _setOutputPortDataType(SimStruct *S, int_T port, int dataTypeId);
int _setInputPortDataType(SimStruct *S, int_T port, int dataTypeId);
int _checkParameters(SimStruct *S);

#ifdef __cplusplus
}
#endif
#endif //SIMEX_BRIDGE_H
