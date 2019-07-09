//
// Created by wuyua on 2019-07-07.
//

#include <cassert>
#include "bridge.h"
#include "block_persistence_registry.h"
#include "block_lifecycle.h"
#include "simstruc.h"
#include "debug_utils.h"
#include "datatype_size.h"

int _initializeMeta(SimStruct *S) {
    auto meta = createBlockMeta(S);
    // Input ports initialization

    DEBUG_ASSERT(ssSetNumInputPorts(S, static_cast<int_T>(meta->inputPorts.size())), \
    "Failed to set number of input ports");

    for (int_T i = 0; i < meta->inputPorts.size(); i++) {
        InputPort &ip = meta->inputPorts[i];
        // TODO: port-based sample time
//        ssSetInputPortOffsetTime(S, i, ip.sampleTime.offset);
//        ssSetInputPortSampleTime(S, i, ip.sampleTime.sampleTime);
        ssSetInputPortFrameData(S, i, ip.acceptFrameData);
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

        DEBUG_ASSERT(ssSetInputPortDimensionInfo(S, i, &di), "Failed to set input port dimension info");
    }

    // Output ports initialization
    DEBUG_ASSERT(ssSetNumOutputPorts(S, static_cast<int_T >(meta->outputPorts.size())), \
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

        DEBUG_ASSERT(ssSetOutputPortDimensionInfo(S, i, &di), "Failed to set output port dimension info");
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

int _initializeSampleTime(SimStruct *S) {
    // TODO: should block meta be cached?
    auto meta = createBlockMeta(S);
    for (int_T i = 0; i < meta->sampleTime.size(); ++i) {
        SampleTime &st = meta->sampleTime[i];
        ssSetSampleTime(S, i, st.sampleTime);
        ssSetOffsetTime(S, i, st.offset);
    }
    return 0;
}


int _start(SimStruct *S) {
    auto &pm = BlockPersistenceRegistry::getOrCreateRegistry(S);
    pm.block = createBlock(S);

    pm.block->onStart();
    return 0;
}

int _terminate(SimStruct *S) {
    auto &pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm.block,
                 "The block is not created when terminating! The resources may not be released. Please clear all to reload the module");
    pm.block->onTerminate();

    BlockPersistenceRegistry::unregister(S);
    return 0;
}

int _output(SimStruct *S) {
    auto &pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT (pm.block, "The block is not created when outputting!");
    pm.block->onOutput();

    for (std::shared_ptr<RuntimeOutputPort> &op : pm.block->outputPorts) {
        bool update = false;
        if (op->autoCopyToSimulink) {
            update = true;
        } else {
            if (op->requestingUpdateToSimulink) {
                update = true;
            }
        }

        if (update) {
            DEBUG_ASSERT(op->data, "Output port has invalid data pointer");
            if (op->data != nullptr) {
                //memcpy(ssGetOutputPortSignal(S, op->portId), op->data, op->length);
                //double* outputPortSignal = (double*)ssGetOutputPortSignal(S, op->portId);
                void* outputPortSignal = ssGetOutputPortSignal(S, op->portId);
                memcpy(outputPortSignal, op->data, op->length);
            }
        }
    }
    return 0;
}

int _update(SimStruct *S) {
    auto &pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm.block, "The block is not created when updating!");
    pm.block->onUpdate();
//    ssPrintf("Port %d: %lf\n", 0,     *ssGetInputPortRealSignalPtrs(S, 0)[0]);
//    ssPrintf("Port %d: %lf\n", 1,     *ssGetInputPortRealSignalPtrs(S, 1)[0]);

    for (std::shared_ptr<RuntimeInputPort> &ip : pm.block->inputPorts) {
        bool update = false;
        if (ip->autoCopyFromSimulink) {
            update = true;
        } else {
            if (ip->requestingUpdateFromSimulink) {
                update = true;
            }
        }

        if (update) {
            int_T el = ssGetInputPortWidth(S, ip->portId);
            int_T sizePerElement = dataTypeIdToByteSize(ssGetInputPortDataType(S, ip->portId));
            int_T totalSize = sizePerElement * el;
            if (ip->length < totalSize) {
                throw std::exception("Input port has insufficient buffer size");
            }

            DEBUG_ASSERT(ip->data, "Input port has invalid data pointer");
            if (ip->data != nullptr) {
                InputPtrsType inputSignalPtrs = ssGetInputPortSignalPtrs(S, ip->portId);
                memcpy(ip->data, *inputSignalPtrs, totalSize);
            }
        }
    }
    return 0;
}