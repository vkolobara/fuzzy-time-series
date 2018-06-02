//
// Created by vkolobara on 6/2/18.
//

#ifndef FUZZY_TIME_SERIES_CLASSIFICATIONLOGGINGOPERATOR_H
#define FUZZY_TIME_SERIES_CLASSIFICATIONLOGGINGOPERATOR_H

#include "ECF/ECF.h"

class ClassificationLoggingOperator : public Operator {
public:
    bool operate(StateP p) override;
};

typedef boost::shared_ptr<ClassificationLoggingOperator> ClassificationLoggingOperatorP;

#endif //FUZZY_TIME_SERIES_CLASSIFICATIONLOGGINGOPERATOR_H
