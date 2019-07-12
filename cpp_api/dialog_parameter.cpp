//
// Created by wuyuanyi on 11/07/19.
//

#include <stdexcept>
#include "dialog_parameter.h"

void DialogParameterBase::onUpdateParameter(const mxArray *data) {
}
DialogParameterBase::DialogParameterBase(int id, bool tunable) : id(id), tunable(tunable) {}
bool DialogParameterBase::onValidateParameter() {
    return true;
}

bool DialogParameter<real64_T>::onValidateParameter() {
    return data > min && data < max;
}

void DialogParameter<real64_T>::onUpdateParameter(const mxArray *newArg) {
    if (!mxIsNumeric(newArg)) throw std::invalid_argument("Dialog Parameter " + std::to_string(id) + " is not numeric");
    this->data = mxGetScalar(newArg);
}
DialogParameter<std::string>::DialogParameter(int id, bool tunable) : DialogParameterBase(id, tunable) {}

DialogParameter<real64_T>::DialogParameter(int id, bool tunable) : DialogParameterBase(id, tunable) {}

bool DialogParameter<std::string>::onValidateParameter() {
    return true;
}

void DialogParameter<std::string>::onUpdateParameter(const mxArray *newArg) {
    char *const str = mxArrayToString(newArg);
    if (!str) {
      throw std::runtime_error("Dialog Parameter " + std::to_string(id) + " is not string");
    }
    data = str;
}