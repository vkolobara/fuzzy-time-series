//
// Created by vkolobara on 5/5/18.
//

#ifndef FUZZY_TIME_SERIES_ERRORFUNCTION_H
#define FUZZY_TIME_SERIES_ERRORFUNCTION_H


class ErrorFunction {
public:
    virtual double error(double real, double pred) = 0;
};

class MeanSquaredError : public ErrorFunction {
public:
    double error(double real, double pred) override;
};

class MeanAbsolutePercentageError : public ErrorFunction {
public:
    double error(double real, double pred) override;
};

class MisclassificationError : public ErrorFunction {
public:
    double error(double real, double pred) override;
};

#endif //FUZZY_TIME_SERIES_ERRORFUNCTION_H
