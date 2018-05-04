//
// Created by vkolobara on 5/4/18.
//

#ifndef FUZZY_TIME_SERIES_CLASSIFICATIONFTSEVALOP_H
#define FUZZY_TIME_SERIES_CLASSIFICATIONFTSEVALOP_H


#include "ClassifierFTSEvalOp.h"

class RegressionFTSEvalOp : public ClassifierFTSEvalOp {

public:
    FitnessP evaluate(IndividualP individual) override;

    bool initialize(StateP p) override;

    shared_ptr<Rule> genotypeToRule(IndividualP individual) override;
};

typedef boost::shared_ptr<RegressionFTSEvalOp> RegressionFTSEvalOpP;

#endif //FUZZY_TIME_SERIES_CLASSIFICATIONFTSEVALOP_H
