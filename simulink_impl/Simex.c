//
// Created by wuyua on 2019-07-07.
//

#include "Simex.h"
#include "bridge.h"

#if defined(MATLAB_MEX_FILE)

static void mdlInitializeSizes(SimStruct *S) {
    _initializeMeta(S);
}

static void mdlInitializeSampleTimes(SimStruct *S) {

}

static void mdlOutputs(SimStruct *S, int_T tid) {

}

static void mdlTerminate(SimStruct *S) {
}

static void mdlUpdate(SimStruct *S, int_T tid) {

}
#endif // #defined(MATLAB_MEX_FILE)

#ifdef MATLAB_MEX_FILE   /* Is this being compiled as MEX-file? */

#include "simulink.c"    /* MEX-file interface mechanism */

#else
#include "cg_sfun.h"     /* Code generation registration func */
#endif