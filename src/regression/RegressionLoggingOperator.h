//
// Created by vkolobara on 3/30/18.
//

#ifndef FUZZY_TIME_SERIES_BESTINDIVIDUALOPERATOR_H
#define FUZZY_TIME_SERIES_BESTINDIVIDUALOPERATOR_H


#include <ECF/ECF.h>
#include "../dataset/Dataset.h"

class RegressionLoggingOperator : public Operator {

    bool operate(StateP state) override;

    double evaluate(StateP state, shared_ptr<Dataset> dataset);
};

typedef boost::shared_ptr<RegressionLoggingOperator> RegressionLoggingOperatorP;


#endif //FUZZY_TIME_SERIES_BESTINDIVIDUALOPERATOR_H
