//
// Created by wuyua on 2019-07-08.
//

#include "dimension_to_width.h"
int dimensionToWidth(std::vector<int>& dimension) {
    if (dimension.size() == 1) {
        return dimension[0];
    } else {
        return dimension[0] * dimension[1];
    }
}