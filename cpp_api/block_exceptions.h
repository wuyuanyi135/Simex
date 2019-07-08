//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BLOCK_EXCEPTIONS_H
#define SIMEX_BLOCK_EXCEPTIONS_H

#include <sstream>

class KeyError : public std::exception {
public:
    void *blockPointer;

    KeyError(void *blockPointer);

    const char *what() const override;
};




#endif //SIMEX_BLOCK_EXCEPTIONS_H
