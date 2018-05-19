//
// Created by Vinko Kolobara on 18. 5. 2018..
//

#ifndef FUZZY_TIME_SERIES_FINTIMESERIESEVALOP_H
#define FUZZY_TIME_SERIES_FINTIMESERIESEVALOP_H

#include "ClassifierFTSEvalOp.h"

class FinTimeSeriesEvalOp : public ClassifierFTSEvalOp {

public:
    FitnessP evaluate(IndividualP individual) override;

    bool initialize(StateP p) override;

    shared_ptr<Rule> genotypeToRule(IndividualP individual) override;
};

typedef boost::shared_ptr<FinTimeSeriesEvalOp> FinTimeSeriesEvalOpP;


#endif //FUZZY_TIME_SERIES_FINTIMESERIESEVALOP_H
