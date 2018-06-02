//
// Created by vkolobara on 6/2/18.
//

#ifndef FUZZY_TIME_SERIES_FINANCIALLOGGINGOPERATOR_H
#define FUZZY_TIME_SERIES_FINANCIALLOGGINGOPERATOR_H


#include <ECF/Operator.h>
#include <boost/shared_ptr.hpp>

class FinancialLoggingOperator : public Operator {
public:
    bool operate(StateP p) override;
};
typedef boost::shared_ptr<FinancialLoggingOperator> FinancialLoggingOperatorP;

#endif //FUZZY_TIME_SERIES_FINANCIALLOGGINGOPERATOR_H
