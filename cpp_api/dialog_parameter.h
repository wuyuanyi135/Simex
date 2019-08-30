//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_DIALOG_PARAMETER_H
#define SIMEX_DIALOG_PARAMETER_H
#include <limits>
#include "string"
#include "simulink_headers.h"
class DialogParameterBase {
 public:
  int id;
  bool tunable;
  virtual void onUpdateParameter(const mxArray *newArg) = 0;
  // If the parameter is not acceptable, return false; Otherwise, modify the value of the data in this callback function.
  virtual bool onValidateParameter() { return true; };
  DialogParameterBase(int id, bool tunable) : id(id), tunable(tunable) {}

  // for serialization
  virtual std::string toString() = 0;
};

template<typename T>
class DialogParameter {};

template<>
class DialogParameter<real64_T> : public DialogParameterBase {
 public:
  DialogParameter(int id, bool tunable): DialogParameterBase(id, tunable) {};
  bool onValidateParameter() override {
      return data > min && data < max;
  }
  void onUpdateParameter(const mxArray *newArg) override {
      if (!mxIsNumeric(newArg)) throw std::invalid_argument("Dialog Parameter " + std::to_string(id) + " is not numeric");
      this->data = mxGetScalar(newArg);
  }
 public:
  real64_T data{0};
  real64_T min{-std::numeric_limits<double>::infinity()};
  real64_T max{std::numeric_limits<double>::infinity()};
  std::string toString() override {
      return std::to_string(data);
  }
};

template<>
class DialogParameter<std::string> : public DialogParameterBase {
 public:
  DialogParameter(int id, bool tunable): DialogParameterBase(id, tunable) {};
  void onUpdateParameter(const mxArray *newArg) override {
      char *const str = mxArrayToString(newArg);
      if (!str) {
          throw std::runtime_error("Dialog Parameter " + std::to_string(id) + " is not string");
      }
      data = str;
  }
  std::string toString() override {
      return data;
  }

 public:
  std::string data{};
};

#endif //SIMEX_DIALOG_PARAMETER_H
