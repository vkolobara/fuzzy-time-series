//
// Created by vkolobara on 5/5/18.
//

#ifndef FUZZY_TIME_SERIES_ERRORFUNCTION_H
#define FUZZY_TIME_SERIES_ERRORFUNCTION_H

#include <string>

class ErrorFunction {
public:
    virtual double error(double real, double pred) = 0;
    virtual std::string getName() = 0;
};

class MeanSquaredError : public ErrorFunction {
public:
    double error(double real, double pred) override;

    std::string getName() override;
};

class MeanAbsolutePercentageError : public ErrorFunction {
public:
    double error(double real, double pred) override;

    std::string getName() override;
};

class MisclassificationError : public ErrorFunction {
public:
    double error(double real, double pred) override;

    std::string getName() override;
};

#endif //FUZZY_TIME_SERIES_ERRORFUNCTION_H
