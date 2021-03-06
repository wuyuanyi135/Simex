//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_DEBUG_UTILS_H
#define SIMEX_DEBUG_UTILS_H

#ifdef DEBUG
#define DEBUG_PRINTF(fmt, ...) ssPrintf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINTF(fmt, ...)
#endif

#if defined(DEBUG) && defined(DEBUG_VERBOSITY_LIFECYCLE)
#define DEBUGV_LIFECYCLE_PRINTF(fmt, ...) ssPrintf(fmt, ##__VA_ARGS__)
#else
#define DEBUGV_LIFECYCLE_PRINTF(fmt, ...)
#endif

#if defined(DEBUG) && defined(DEBUG_SIMULINK_CALLBACK)
#define DEBUGV_SIMULINKCB_PRINTF(fmt, ...) ssPrintf(fmt, ##__VA_ARGS__)
#else
#define DEBUGV_SIMULINKCB_PRINTF(fmt, ...)
#endif

#if defined(DEBUG) && !defined(DISABLE_ASSERT)
#define DEBUG_ASSERT(C, MSG) \
do {\
    if (!C) {\
        throw std::runtime_error(MSG); \
    } \
} while(0)
#else
#define DEBUG_ASSERT(C, MSG) C;
#endif
#endif //SIMEX_DEBUG_UTILS_H

