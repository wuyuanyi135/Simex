//
// Created by wuyuanyi on 11/07/19.
//

#ifndef SIMEX_DYNAMIC_DATA_H
#define SIMEX_DYNAMIC_DATA_H

#include "simstruc.h"

class DynamicData {
 public:
  void *data{nullptr};
  int size{0};

  virtual ~DynamicData() { delete[] static_cast<uint8_T*>(data); }

  void reallocate(int sz) {
      if (sz != size) {
          delete[] static_cast<uint8_T*>(data);
          data = new uint8_T[sz]();
          size = sz;
      }
  }
};

#endif //SIMEX_DYNAMIC_DATA_H
