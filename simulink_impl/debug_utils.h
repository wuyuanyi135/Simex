//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_DEBUG_UTILS_H
#define SIMEX_DEBUG_UTILS_H

#include "simstruc.h"
#ifdef DEBUG
#define DEBUG_PRINTF(fmt, ...) ssPrintf(fmt, __VA_ARGS__)
#else
#define DEBUG_PRINTF(fmt, ...)
#endif

#if defined(DEBUG) && defined(DEBUG_VERBOSITY_LIFECYCLE)
#define DEBUGV_LIFECYCLE_PRINTF(fmt, ...) ssPrintf(fmt, __VA_ARGS__)
#else
#define DEBUGV_LIFECYCLE_PRINTF(fmt, ...)
#endif

#if defined(DEBUG) && defined(DEBUG_SIMULINK_CALLBACK)
#define DEBUGV_SIMULINKCB_PRINTF(fmt, ...) ssPrintf(fmt, __VA_ARGS__)
#else
#define DEBUGV_SIMULINKCB_PRINTF(fmt, ...)
#endif

#if defined(DEBUG) && !defined(DISABLE_ASSERT)
#define DEBUG_ASSERT(C, MSG) \
if (!C) {\
    throw std::exception(MSG); \
}
#else
#define DEBUG_ASSERT(C, MSG)
#endif
#endif //SIMEX_DEBUG_UTILS_H

