//
// Created by wuyuanyi on 11/07/19.
//

#ifndef SIMEX_DYNAMIC_DATA_H
#define SIMEX_DYNAMIC_DATA_H

#include <tmwtypes.h>

#include <memory>
#include "simulink_headers.h"

class DynamicData {
public:
    DynamicData() = default;

    DynamicData(const DynamicData& ref) {
        // share the pointer. do not copy the underlying buffer.
        size = ref.size;
        data = ref.data;
    }
public:
    std::shared_ptr<char[]> data{nullptr};
    int size{0};

//    virtual ~DynamicData() { delete[] static_cast<uint8_T *>(data); }

    void reallocate(int sz) {
        if (sz != size) {
            size = sz;
            data.reset(new char[size]());
        }
    }
};

#endif //SIMEX_DYNAMIC_DATA_H
