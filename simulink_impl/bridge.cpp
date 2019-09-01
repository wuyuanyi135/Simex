//
// Created by wuyua on 2019-07-07.
//

#include "bridge.h"
#include "dimension_to_width.h"
#include "block_persistence_registry.h"
#include "block_lifecycle.h"
#include "debug_utils.h"
#include "datatype_size.h"
#include "simstruc.h"

#define MAX_DIM 10

int _initializeMeta(SimStruct *S) {
    // Input ports initialization
    auto pm = BlockPersistenceRegistry::getOrCreateRegistry(S);

    // Will overwrite any previous block meta data (data for initialization the size).
    pm->block = createBlock(S);
    auto &blk = pm->block;
    if (blk->allowSignalsWithMoreThan2D) {
        ssAllowSignalsWithMoreThan2D(S);
    }

    DEBUG_ASSERT(ssSetNumInputPorts(S, static_cast<int_T>(blk->inputPorts.size())), \
    "Failed to set number of input ports");

    for (int_T i = 0; i < blk->inputPorts.size(); i++) {
        auto &ip = blk->inputPorts[i];
        // TODO: port-based sample time
        // ssSetInputPortOffsetTime(S, i, ip.sampleTime.offset);
        // ssSetInputPortSampleTime(S, i, ip.sampleTime.sampleTime);
        ssSetInputPortFrameData(S, i, ip->acceptFrameData);
        ssSetInputPortComplexSignal(S, i, ip->complexity);
        ssSetInputPortDirectFeedThrough(S, i, ip->directFeedthrough);
        ssSetInputPortDataType(S, i, ip->dataTypeId);

        if (ip->dynamicDimension) {
            DEBUG_ASSERT(ssSetInputPortDimensionInfo(S, i, DYNAMIC_DIMENSION),
                         "Failed to set dynamic dimension input port dimension info");
        } else {
            DECL_AND_INIT_DIMSINFO(di);
            auto sz = static_cast<int_T>(ip->dimension.size());
            int_T dims[MAX_DIM];

            if (sz > MAX_DIM) {
                throw std::range_error("Too many dimensions (sz > MAX_DIM)");
            }

            for (int i = 0; i < sz; i++) {
                dims[i] = ip->dimension[i];
            }

            di.width = dimensionToWidth(ip->dimension);
            di.numDims = sz;
            di.dims = dims;
            DEBUG_ASSERT(ssSetInputPortDimensionInfo(S, i, &di), "Failed to set input port dimension info");
        }

    }

    // Output ports initialization
    DEBUG_ASSERT(ssSetNumOutputPorts(S, static_cast<int_T >(blk->outputPorts.size())), \
    "Failed to set number of output ports");
    DEBUG_PRINTF("Output port size: %d\n", blk->outputPorts.size());
    for (int i = 0; i < blk->outputPorts.size(); i++) {
        auto &op = blk->outputPorts[i];
        // TODO: port-based sample time
        // ssSetOutputPortOffsetTime(S, i, op.sampleTime.offset);
        // ssSetOutputPortSampleTime(S, i, op.sampleTime.sampleTime);
        ssSetOutputPortFrameData(S, i, op->acceptFrameData);
        ssSetOutputPortComplexSignal(S, i, op->complexity);
        ssSetOutputPortDataType(S, i, op->dataTypeId);

        if (op->dynamicDimension) {
            DEBUG_ASSERT(ssSetOutputPortDimensionInfo(S, i, DYNAMIC_DIMENSION),
                         "Failed to set dynamic dimension output port dimension info");
        } else {

            DECL_AND_INIT_DIMSINFO(di);
            auto sz = static_cast<int_T>(op->dimension.size());

            int_T dims[MAX_DIM];

            if (sz > MAX_DIM) {
                throw std::range_error("Too many dimensions (sz > MAX_DIM)");
            }

            for (int i = 0; i < sz; i++) {
                dims[i] = op->dimension[i];
            }

            di.width = dimensionToWidth(op->dimension);
            di.numDims = sz;
            di.dims = dims;
            DEBUG_ASSERT(ssSetOutputPortDimensionInfo(S, i, &di), "Failed to set output port dimension info");
        }

    }

    // Number of parameters
    DEBUG_PRINTF("Dialog parameters: %d\n", blk->dialogParameters.size());
    ssSetNumSFcnParams(S, static_cast<int_T>(blk->dialogParameters.size()));
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != NULL) {
            throw std::invalid_argument(ssGetErrorStatus(S));
        }
    } else {
        return -1; /* Parameter mismatch reported by the Simulink engine*/
    }

    for (int i = 0; i < blk->dialogParameters.size(); i++) {
        ssSetSFcnParamTunable(S, i, blk->dialogParameters[i]->tunable ? SS_PRM_TUNABLE : SS_PRM_NOT_TUNABLE);
    }
    // Number of states
    ssSetNumContStates(S, blk->numContinuousStates);
    ssSetNumDiscStates(S, blk->numDiscreteStates);

    ssSetNumSampleTimes(S, static_cast<int_T>(blk->sampleTime.size()));

    ssSetNumRWork(S, blk->numRWorks);
    ssSetNumIWork(S, blk->numIWorks);
    ssSetNumPWork(S, blk->numPWorks);
    ssSetNumModes(S, blk->numModes);
    ssSetNumNonsampledZCs(S, blk->numNonsampledZeroCrossings);
    ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);
    ssSetOptions(S, blk->options);

    return 0;
}

int _initializeSampleTime(SimStruct *S) {
    auto pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm->block, "Block blk is not found when initialize sample time");
    auto &blk = pm->block;
    for (int_T i = 0; i < blk->sampleTime.size(); ++i) {
        SampleTime &st = blk->sampleTime[i];
        ssSetSampleTime(S, i, st.sampleTime);
        ssSetOffsetTime(S, i, st.offset);
    }
    return 0;
}

int _start(SimStruct *S) {
    auto pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm->block, "block meta not exist when start");
    pm->block->onInitializeRuntime();
    pm->block->isStarted = true;
    pm->block->onStart();
    return 0;
}

int _terminate(SimStruct *S) {
    auto pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm->block,
                 "The block is not created when terminating! The resources may not be released. Please clear all to reload the module");
    try {
        pm->block->onTerminate();
    } catch (std::exception &) {

    }
    pm->block->isStarted = false;
    BlockPersistenceRegistry::unregister(S);
    return 0;
}

int _output(SimStruct *S) {
    auto pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT (pm->block, "The block is not created when outputting!");
    std::lock_guard<std::mutex> lock(pm->block->mainThreadAccessMutex);
    // direct feedthrough should be updated prior to the callback.
    for (std::shared_ptr<InputPort> &ip : pm->block->inputPorts) {
        bool update = false;
        if (ip->autoCopyFromSimulink) {
            update = true;
        } else {
            if (ip->requestingUpdateFromSimulink) {
                update = true;
            }
        }
        if (update && ip->directFeedthrough) {
            auto el = ssGetInputPortWidth(S, ip->portId);
            int_T sizePerElement = dataTypeIdToByteSize(ssGetInputPortDataType(S, ip->portId));
            int_T totalSize = sizePerElement * el;
            DEBUG_ASSERT(ip->portData.getSize() == totalSize, "Input port buffer size mismatch!");
            // Copy directFeedthrough data to input ports.
            InputPtrsType inputSignalPtrs = ssGetInputPortSignalPtrs(S, ip->portId);
            ip->portData.copyFrom(*inputSignalPtrs, totalSize);
        }
    }
    pm->block->onOutput();

    for (std::shared_ptr<OutputPort> &op : pm->block->outputPorts) {
        bool update = false;
        if (op->autoCopyToSimulink) {
            update = true;
        } else {
            if (op->requestingUpdateToSimulink) {
                update = true;
            }
        }

        if (update) {
            void *outputPortSignal = ssGetOutputPortSignal(S, op->portId);
            op->portData.copyTo(outputPortSignal, op->portData.getSize());
        }
    }
    return 0;
}

int _update(SimStruct *S) {
    auto pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm->block, "The block is not created when updating!");
//    ssPrintf("Port %d: %lf\n", 0,     *ssGetInputPortRealSignalPtrs(S, 0)[0]);
//    ssPrintf("Port %d: %lf\n", 1,     *ssGetInputPortRealSignalPtrs(S, 1)[0]);
    std::lock_guard<std::mutex> lock(pm->block->mainThreadAccessMutex);
    for (std::shared_ptr<InputPort> &ip : pm->block->inputPorts) {
        bool update = false;
        if (ip->autoCopyFromSimulink) {
            update = true;
        } else {
            if (ip->requestingUpdateFromSimulink) {
                update = true;
            }
        }

        if (update && !ip->directFeedthrough) {
            int_T el = ssGetInputPortWidth(S, ip->portId);
            int_T sizePerElement;
            sizePerElement = dataTypeIdToByteSize(ssGetInputPortDataType(S, ip->portId));
            int_T totalSize = sizePerElement * el;
            DEBUG_ASSERT(ip->portData.getSize() == totalSize, "Input port buffer size mismatch!");

            InputPtrsType inputSignalPtrs = ssGetInputPortSignalPtrs(S, ip->portId);
            ip->portData.copyFrom(*inputSignalPtrs, totalSize);
        }
    }

    pm->block->onUpdate();

    return 0;
}

int _setOutputPortDimensionInfo(SimStruct *S, int_T port, const DimsInfo_T *dims) {
    auto pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm->block, "Block is not created in output dimension info callback");

    auto &p = pm->block->outputPorts[port];
    p->dimension.clear();

    for (int i = 0; i < dims->numDims; ++i) {
        p->dimension.push_back(dims->dims[i]);
    }
    int size = dimensionToWidth(p->dimension) * dataTypeIdToByteSize(p->dataTypeId);

    if (pm->block->outputPorts[port]->validateDimension()) {
        ssSetOutputPortDimensionInfo(S, port, dims);
        p->portData.resize(size);
    } else {
        throw std::invalid_argument("Output port dimension error");
    }
    return 0;
}

int _setInputPortDimensionInfo(SimStruct *S, int_T port, const DimsInfo_T *dims) {
    auto pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm->block, "Block is not created in input dimension info callback");

    auto &p = pm->block->inputPorts[port];
    p->dimension.clear();

    for (int i = 0; i < dims->numDims; ++i) {
        p->dimension.push_back(dims->dims[i]);
    }
    int size = dimensionToWidth(p->dimension) * dataTypeIdToByteSize(p->dataTypeId);

    if (pm->block->inputPorts[port]->validateDimension()) {
        ssSetInputPortDimensionInfo(S, port, dims);
        p->portData.resize(size);
    } else {
        throw std::invalid_argument("Input port dimension error");
    }
    return 0;
}

int _setOutputPortDataType(SimStruct *S, int_T port, int dataTypeId) {
    auto pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm->block, "Block is not created in output data type callback");
    auto &p = pm->block->outputPorts[port];
    p->dataTypeId = dataTypeId;
    if (p->validateDataType()) {
        ssSetOutputPortDataType(S, port, dataTypeId);
    } else {
        throw std::invalid_argument("Output port data type id error");
    }
    return 0;
}

int _setInputPortDataType(SimStruct *S, int_T port, int dataTypeId) {
    auto pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm->block, "Block is not created in input data type callback");
    auto &p = pm->block->inputPorts[port];
    p->dataTypeId = dataTypeId;
    if (p->validateDataType()) {
        ssSetInputPortDataType(S, port, dataTypeId);
    } else {
        throw std::invalid_argument("Input port data type id error");
    }
    return 0;
}

int _checkParameters(SimStruct *S) {
    auto pm = BlockPersistenceRegistry::getRegistry(S);
    DEBUG_ASSERT(pm->block, "Block is not created in check parameter");
    int numParams = ssGetSFcnParamsCount(S);
    int numExpected = pm->block->dialogParameters.size();
    // TODO: when except does it release
    if (numParams != numExpected)
        throw std::invalid_argument("Dialog parameter number mismatch. Expect " + std::to_string(numExpected) + ". Got "
                                    + std::to_string(numParams));

    for (int i = 0; i < numExpected; i++) {
        pm->block->dialogParameters[i]->onUpdateParameter(ssGetSFcnParam(S, i));
        if (!pm->block->dialogParameters[i]->onValidateParameter())
            throw std::invalid_argument("Parameter " + std::to_string(i) + " validation failed!");
    }

    pm->block->onParameterUpdated();
    return 0;
}