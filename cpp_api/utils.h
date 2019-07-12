//
// Created by wuyua on 2019-07-09.
//

#ifndef SIMEX_UTILS_H
#define SIMEX_UTILS_H

#define BLOCK_PTR(S) reinterpret_cast<intptr_t >(S->blkInfo.block)
#define LOCK_WEAK_PTR_OR_EXCEPTION(WPTR, SHARED_PTR_NAME) \
auto& SHARED_PTR_NAME = WPTR.lock(); \
do { \
    if (!SHARED_PTR_NAME) throw std::runtime_error("Failed to lock a weak pointer"); \
} while(0)
#endif //SIMEX_UTILS_H
