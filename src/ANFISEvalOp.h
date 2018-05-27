//
// Created by Vinko Kolobara on 27. 5. 2018..
//

#ifndef FUZZY_TIME_SERIES_ANFISEVALOP_H
#define FUZZY_TIME_SERIES_ANFISEVALOP_H

#include "ECF/ECF.h"
#include "dataset/Dataset.h"
#include "ClassifierFTSEvalOp.h"
#include "ANFIS/layer/ANFIS.h"

class ANFISEvalOp : public ClassifierFTSEvalOp {

public:

    FitnessP evaluate(IndividualP individual) override;

    bool initialize(StateP p) override;

    //shared_ptr<Rule> genotypeToRule(IndividualP individual) override;

    shared_ptr<ANFIS> genotypeToANFIS(IndividualP individual);

};

typedef boost::shared_ptr<ANFISEvalOp> ANFISEvalOpP;

#endif //FUZZY_TIME_SERIES_ANFISEVALOP_H
