//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#ifndef FUZZY_TIME_SERIES_FTSEVALOP_H
#define FUZZY_TIME_SERIES_FTSEVALOP_H

#include <ECF/ECF.h>
#include "FIS/parser/VariableParser.h"
#include "dataset/Dataset.h"
#include "FIS/inference/Defuzzifier.h"
#include "FIS/inference/InferenceSystem.h"

class FTSEvalOp : public EvaluateOp {
private:
    VariableParser* variableParser;
    Dataset* dataset;
    vector<std::string> variableNames;
    Defuzzifier* defuzzifier;

    MamdaniInferenceSystem genotypeToInferenceSystem(IndividualP individual);

public:
    FitnessP evaluate(IndividualP individual) override;
    void registerParameters(StateP) override;
    bool initialize(StateP) override;
};
typedef boost::shared_ptr<FTSEvalOp> FTSEvalOpP;


#endif //FUZZY_TIME_SERIES_FTSEVALOP_H
