//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_DIALOG_PARAMETER_H
#define SIMEX_DIALOG_PARAMETER_H
#include <limits>
#include "string"
#include "simstruc.h"
class DialogParameterBase {
 public:
  int id{false};
  bool tunable{false};
  virtual void onUpdateParameter(const mxArray *newArg);
  // If the parameter is not acceptable, return false; Otherwise, modify the value of the data in this callback function.
  virtual bool onValidateParameter();
  DialogParameterBase(int id, bool tunable);
};

template<typename T>
class DialogParameter {};

template<>
class DialogParameter<real64_T> : public DialogParameterBase {
 public:
  DialogParameter(int id, bool tunable);
  void onUpdateParameter(const mxArray *newArg) override;
  bool onValidateParameter() override;
  real64_T data{0};
 public:
  real64_T min{-std::numeric_limits<double>::infinity()};
  real64_T max{std::numeric_limits<double>::infinity()};

};

template<>
class DialogParameter<std::string> : public DialogParameterBase {
 public:
  void onUpdateParameter(const mxArray *newArg) override;
  DialogParameter(int id, bool tunable);
  bool onValidateParameter() override;
  std::string data{};
};

#endif //SIMEX_DIALOG_PARAMETER_H
