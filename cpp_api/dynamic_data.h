//
// Created by wuyuanyi on 11/07/19.
//

#ifndef SIMEX_DYNAMIC_DATA_H
#define SIMEX_DYNAMIC_DATA_H

#include <tmwtypes.h>
#include <mutex>
#include <memory>
#include "simulink_headers.h"

class DynamicData {
public:
    explicit DynamicData(int sz) { reallocate(sz); }

    DynamicData(const DynamicData &ref) {
        // share the pointer. do not copy the underlying buffer.
        size = ref.size;
        data = ref.data;
    }

private:
    std::shared_ptr<char[]> data{nullptr};
    int size{0};
    std::mutex mutex{};

//    virtual ~DynamicData() { delete[] static_cast<uint8_T *>(data); }

    void reallocate(int sz) {
        if (!sz) return;
        if (sz != size) {
            size = sz;
            data.reset(new char[size]());
        }
    }

public:

    int getSize() const {
        return size;
    }

    void copyTo(void *dest, int sz = -1) {
        std::lock_guard<std::mutex> lock(mutex);
        DEBUG_ASSERT(dest, "copyTo dest == NULL");
        if (sz < 0) {
            sz = size;
        }
        memcpy(dest, data.get(), sz);
    }

    void copyFrom(DynamicData &ref) {
        int sz = ref.getSize();
        reallocate(sz);
        ref.copyTo(data.get(), sz);
    }

    void copyFrom(const void *src, int sz = -1) {
        std::lock_guard<std::mutex> lock(mutex);
        if (sz < 0) {
            sz = size;
        } else {
            reallocate(sz);
        }

        memcpy(data.get(), src, sz);
    }

    // Should only be called in dimension callbacks!
    void resize(int sz) {
        reallocate(sz);
    }

    std::string toString() {
        std::lock_guard<std::mutex> lock(mutex);
        return std::string(data.get(), size);
    }

    template<typename RetType>
    RetType as() {
        RetType ret;
        copyTo(&ret, sizeof(ret));
        return ret;
    }

    template<typename SetType>
    void set(const SetType &target) {
        copyFrom(&target, sizeof(target));
    }

    template<typename RetType>
    std::unique_ptr<std::vector<RetType *>> getDataView() {
        auto p = std::unique_ptr<std::vector<RetType *>>(new std::vector<RetType *>());
        if (size % sizeof(RetType) != 0) {
            throw std::runtime_error("size is not divisible by RetType");
        }
        for (int i = 0; i < size / sizeof(RetType); i++) {
            void* ptr = data.get() + sizeof(RetType)*i;
            p->emplace_back(static_cast<RetType*>(ptr));
        }
        return p;
    };

    char *operator+(int offset) {
        return (static_cast<char *>(data.get()) + offset);
    }

    char &operator[](int index) {
        return *operator+(index);
    }


};

#endif //SIMEX_DYNAMIC_DATA_H
