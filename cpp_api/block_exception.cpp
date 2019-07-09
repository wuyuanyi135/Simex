//
// Created by wuyua on 2019-07-07.
//
#include "block_exceptions.h"


const char *KeyError::what() const noexcept {
    std::stringstream ss;
    ss << "key error: " <<blockPointer;
    return ss.str().c_str();
}

KeyError::KeyError(void *blockPointer) : blockPointer(blockPointer) {

}
