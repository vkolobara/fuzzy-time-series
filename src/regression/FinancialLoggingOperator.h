//
// Created by vkolobara on 6/2/18.
//

#ifndef FUZZY_TIME_SERIES_REGRESSION2LOGGINGOPERATOR_H
#define FUZZY_TIME_SERIES_REGRESSION2LOGGINGOPERATOR_H


#include <ECF/ECF.h>

class Regression2LoggingOperator : public Operator {
public:
    bool operate(StateP p) override;
};
typedef boost::shared_ptr<Regression2LoggingOperator> Regression2LoggingOperatorP;

#endif //FUZZY_TIME_SERIES_FINANCIALLOGGINGOPERATOR_H
