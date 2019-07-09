//
// Created by wuyua on 2019-07-08.
//

#include <exception>
#include <string>
#include <stdexcept>
#include "datatype_size.h"

int dataTypeIdToByteSize(int dataTypeId) {
    switch (dataTypeId) {
        case 0:
            return sizeof(real64_T);

        case 1:
            return sizeof(real32_T);

        case 2:
            return sizeof(int8_T);

        case 3:
            return sizeof(uint8_T);

        case 4:
            return sizeof(int16_T);

        case 5:
            return sizeof(uint16_T);

        case 6:
            return sizeof(int32_T);

        case 7:
            return sizeof(uint32_T);

        case 8:
            return sizeof(boolean_T);

        default:
            std::string s = "Unknown datatype id: " + std::to_string(dataTypeId);
            throw std::invalid_argument(s.c_str());
    }
}