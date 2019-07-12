//
// Created by wuyua on 2019-07-08.
//

#include "dimension_to_width.h"
int dimensionToWidth(std::vector<int>& dimension) {
    int width = 1;
    for (int el : dimension) {
        width *= el;
    }
    return width;
}