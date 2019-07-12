//
// Created by wuyua on 2019-07-07.
//

#include "simulink_callbacks.h"
#include "bridge.h"
#include "debug_utils.h"

#if defined(MATLAB_MEX_FILE)

static void mdlInitializeSizes(SimStruct *S) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlInitializeSizes\n");
    _initializeMeta(S);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlInitializeSizes\n");
}

static void mdlInitializeSampleTimes(SimStruct *S) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlInitializeSampleTimes\n");
    _initializeSampleTime(S);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlInitializeSampleTimes\n");
}

#define MDL_START
static void mdlStart(SimStruct *S) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlStart\n");
    _start(S);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlStart\n");
}

static void mdlOutputs(SimStruct *S, int_T tid) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlOutputs\n");
    _output(S);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlOutputs\n");
}

static void mdlTerminate(SimStruct *S) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlTerminate\n");
    _terminate(S);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlTerminate\n");
}

#define MDL_UPDATE
static void mdlUpdate(SimStruct *S, int_T tid) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlUpdate\n");
    _update(S);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlUpdate\n");
}

#define MDL_SET_INPUT_PORT_DIMENSION_INFO
static void mdlSetInputPortDimensionInfo(SimStruct *S, int_T port, const DimsInfo_T *dimsInfo) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlSetInputPortDimensionInfo\n");
    _setInputPortDimensionInfo(S, port, dimsInfo);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlSetInputPortDimensionInfo\n");
}
#define MDL_SET_OUTPUT_PORT_DIMENSION_INFO
static void mdlSetOutputPortDimensionInfo(SimStruct *S, int_T port, const DimsInfo_T *dimsInfo) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlSetOutputPortDimensionInfo\n");
    _setOutputPortDimensionInfo(S, port, dimsInfo);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlSetOutputPortDimensionInfo\n");
}

#define MDL_SET_INPUT_PORT_DATA_TYPE
static void mdlSetInputPortDataType(SimStruct *S, int_T port, DTypeId id) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlSetInputPortDataType\n");
    _setInputPortDataType(S, port, id);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlSetInputPortDataType\n");
}

/* for dynamic typed ports */
#define MDL_SET_OUTPUT_PORT_DATA_TYPE
static void mdlSetOutputPortDataType(SimStruct *S, int_T port, DTypeId id) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlSetOutputPortDataType\n");
    _setOutputPortDataType(S, port, id);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlSetOutputPortDataType\n");

}

#define MDL_CHECK_PARAMETERS
void mdlCheckParameters(SimStruct *S) {
    DEBUGV_SIMULINKCB_PRINTF("Entered mdlCheckParameters\n");
    _checkParameters(S);
    DEBUGV_SIMULINKCB_PRINTF("Exited mdlCheckParameters\n");
}

#endif // #defined(MATLAB_MEX_FILE)

#ifdef MATLAB_MEX_FILE   /* Is this being compiled as MEX-file? */

#include "simulink.c"    /* MEX-file interface mechanism */

#else
#include "cg_sfun.h"     /* Code generation registration func */
#endif