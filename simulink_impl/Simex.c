//
// Created by wuyua on 2019-07-07.
//

#include "Simex.h"
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
#endif // #defined(MATLAB_MEX_FILE)

#ifdef MATLAB_MEX_FILE   /* Is this being compiled as MEX-file? */

#include "simulink.c"    /* MEX-file interface mechanism */

#else
#include "cg_sfun.h"     /* Code generation registration func */
#endif