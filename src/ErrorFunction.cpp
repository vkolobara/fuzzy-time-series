//
// Created by vkolobara on 5/5/18.
//

#include <cmath>
#include "ErrorFunction.h"

double MeanSquaredError::error(double real, double pred) {
    return (real - pred) * (real - pred);
}

std::string MeanSquaredError::getName() {
    return "mse";
}

double MeanAbsolutePercentageError::error(double real, double pred) {
    return std::abs((real - pred) / real);
}

std::string MeanAbsolutePercentageError::getName() {
    return "mape";
}

double MisclassificationError::error(double real, double pred) {
    return (int) real == (int) pred ? 0 : 1;
}

std::string MisclassificationError::getName() {
    return "zeroone";
}
