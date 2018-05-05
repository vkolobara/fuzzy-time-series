//
// Created by vkolobara on 5/5/18.
//

#include <cmath>
#include "ErrorFunction.h"

double MeanSquaredError::error(double real, double pred) {
    return (real - pred) * (real - pred);
}

double MeanAbsolutePercentageError::error(double real, double pred) {
    return std::abs((real - pred) / real);
}

double MisclassificationError::error(double real, double pred) {
    return (int) real == (int) pred ? 0 : 1;
}
