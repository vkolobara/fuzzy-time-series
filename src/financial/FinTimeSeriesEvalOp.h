//
// Created by Vinko Kolobara on 18. 5. 2018..
//

#ifndef FUZZY_TIME_SERIES_FINTIMESERIESEVALOP_H
#define FUZZY_TIME_SERIES_FINTIMESERIESEVALOP_H

#include "../classification/ClassifierFTSEvalOp.h"

class FinTimeSeriesEvalOp : public ClassifierFTSEvalOp {

public:
    double startBalance;

    double threshold;

    uint counter = 0;

    FitnessP evaluate(IndividualP individual) override;

    FitnessP evaluate(IndividualP individual, shared_ptr<Dataset> dataset, int currentSplit, int numSplits, bool train);

    bool initialize(StateP p) override;

    void registerParameters(StateP) override;
    //shared_ptr<Rule> genotypeToRule(IndividualP individual) override;

    vector<shared_ptr<Rule>> genotypeToRules(IndividualP individual) override;

};

typedef boost::shared_ptr<FinTimeSeriesEvalOp> FinTimeSeriesEvalOpP;


#endif //FUZZY_TIME_SERIES_FINTIMESERIESEVALOP_H
