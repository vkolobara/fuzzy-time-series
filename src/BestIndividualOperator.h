//
// Created by vkolobara on 3/30/18.
//

#ifndef FUZZY_TIME_SERIES_BESTINDIVIDUALOPERATOR_H
#define FUZZY_TIME_SERIES_BESTINDIVIDUALOPERATOR_H


#include <ECF/ECF.h>

class BestIndividualOperator : public Operator {

    bool operate(StateP state) override;
};

typedef boost::shared_ptr<BestIndividualOperator> BestIndividualOperatorP;


#endif //FUZZY_TIME_SERIES_BESTINDIVIDUALOPERATOR_H
