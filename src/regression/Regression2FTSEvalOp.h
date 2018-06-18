//
// Created by vkolobara on 5/4/18.
//

#ifndef FUZZY_TIME_SERIES_CLASSIFICATIONFTSEVALOP_H
#define FUZZY_TIME_SERIES_CLASSIFICATIONFTSEVALOP_H


#include "../classification/ClassifierFTSEvalOp.h"
#include "RegressionRules.h"

class Regression2FTSEvalOp : public ClassifierFTSEvalOp {

public:
    shared_ptr<RegressionRules> rules;
    FitnessP evaluate(IndividualP individual) override;

    bool initialize(StateP p) override;

    vector<shared_ptr<Rule>> genotypeToRules(IndividualP individual) override;

    FitnessP evaluate(IndividualP individual, shared_ptr<Dataset> dataset) override;

    void registerParameters(StateP) override;
};

typedef boost::shared_ptr<Regression2FTSEvalOp> Regression2FTSEvalOpP;

#endif //FUZZY_TIME_SERIES_CLASSIFICATIONFTSEVALOP_H
