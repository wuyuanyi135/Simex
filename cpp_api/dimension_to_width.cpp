//
// Created by wuyuanyi on 2019-08-19.
//
#include "dimension_to_width.h"
int dimensionToWidth(std::vector<int>& dimension) {
    int width = 1;
    for (int el : dimension) {
        width *= el;
    }
    return width;
}

int dimensionToWidth(const int *dimension, int size) {
    int width = 1;
    for (int i = 0; i < size; i ++) {
        width *= dimension[i];
    }
    return width;
}
