//
// Created by wuyua on 2019-07-07.
//

#include "bridge.h"
#include "block_persistence_registry.h"
#include "block_lifecycle.h"
#include "simstruc.h"
#include "debug_utils.h"

int _initializeMeta(SimStruct *S) {
    auto meta = createBlockMeta(S);
    // Input ports initialization
    mxAssert(ssSetNumInputPorts(S, static_cast<int_T>(meta->inputPorts.size())),  \
    "Failed to set number of input ports");

    for (int_T i = 0; i < meta->inputPorts.size(); i++) {
        InputPort &ip = meta->inputPorts[i];
        // TODO: port-based sample time
//        ssSetInputPortOffsetTime(S, i, ip.sampleTime.offset);
//        ssSetInputPortSampleTime(S, i, ip.sampleTime.sampleTime);
        ssSetInputPortFrameData(S, i, ip.acceptFrameData), "Failed to set input port frame data";
        ssSetInputPortComplexSignal(S, i, ip.complexity);
        ssSetInputPortDirectFeedThrough(S, i, ip.directFeedthrough);
        ssSetInputPortDataType(S, i, ip.dataTypeId);

        DECL_AND_INIT_DIMSINFO(di);
        auto sz = static_cast<int_T>(ip.dimension.size());
        int_T dims[2];

        if (sz == 2) {
            di.width = ip.dimension[0] * ip.dimension[1];
            dims[0] = ip.dimension[0];
            dims[1] = ip.dimension[1];
        } else {
            di.width = ip.dimension[0];
            dims[0] = ip.dimension[0];
        }
        di.numDims = sz;
        di.dims = dims;

        mxAssert(ssSetInputPortDimensionInfo(S, i, &di), "Failed to set input port dimension info");
    }

    // Output ports initialization
    mxAssert(ssSetNumOutputPorts(S, static_cast<int_T >(meta->outputPorts.size())), \
    "Failed to set number of output ports");
    DEBUG_PRINTF("Output port size: %d\n", meta->outputPorts.size());
    for (int i = 0; i < meta->outputPorts.size(); i++) {
        OutputPort &op = meta->outputPorts[i];
        // TODO: port-based sample time
//        ssSetOutputPortOffsetTime(S, i, op.sampleTime.offset);
//        ssSetOutputPortSampleTime(S, i, op.sampleTime.sampleTime);
        ssSetOutputPortFrameData(S, i, op.acceptFrameData);
        ssSetOutputPortComplexSignal(S, i, op.complexity);
        ssSetOutputPortDataType(S, i, op.dataTypeId);

        DECL_AND_INIT_DIMSINFO(di);
        auto sz = static_cast<int_T>(op.dimension.size());
        int_T dims[2];

        if (sz == 2) {
            di.width = op.dimension[0] * op.dimension[1];
            dims[0] = op.dimension[0];
            dims[1] = op.dimension[1];
        } else {
            di.width = op.dimension[0];
            dims[0] = op.dimension[0];
        }
        di.numDims = sz;
        di.dims = dims;

        mxAssert(ssSetOutputPortDimensionInfo(S, i, &di), "Failed to set output port dimension info");
    }

    // Number of parameters
    DEBUG_PRINTF("Dialog parameters: %d\n", meta->dialogParameters.size());
    ssSetNumSFcnParams(S, static_cast<int_T>(meta->dialogParameters.size()));
    for (int i = 0; i < meta->dialogParameters.size(); i++) {
        ssSetSFcnParamTunable(S, i, meta->dialogParameters[i].tunable);
    }

    // Number of states
    ssSetNumContStates(S, meta->numContinuousStates);
    ssSetNumDiscStates(S, meta->numDiscreteStates);

    ssSetNumSampleTimes(S, static_cast<int_T>(meta->sampleTime.size()));

    ssSetNumRWork(S, meta->numRWorks);
    ssSetNumIWork(S, meta->numIWorks);
    ssSetNumPWork(S, meta->numPWorks);
    ssSetNumModes(S, meta->numModes);
    ssSetNumNonsampledZCs(S, meta->numNonsampledZeroCrossings);
    ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);
    ssSetOptions(S, meta->options);
    return 0;
}

int _initialize(SimStruct *S) {

    return 0;
}

int _terminate(SimStruct *S) {
    return 0;
}