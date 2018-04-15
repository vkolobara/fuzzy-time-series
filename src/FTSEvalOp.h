//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#ifndef FUZZY_TIME_SERIES_FTSEVALOP_H
#define FUZZY_TIME_SERIES_FTSEVALOP_H

#include <ECF/ECF.h>
#include <FIS/rule/Defuzzifier.h>
#include <FIS/rule/InferenceSystem.h>
#include "FIS/parser/VariableParser.h"
#include "dataset/Dataset.h"

class FTSEvalOp : public EvaluateOp {
public:
    VariableParser* variableParser;
    Dataset* dataset;
    vector<std::string> variableNames;
    Defuzzifier* defuzzifier;

    uint numRules;
    uint numVars;

public:
    FitnessP evaluate(IndividualP individual) override;
    void registerParameters(StateP) override;
    bool initialize(StateP) override;
    InferenceSystem* genotypeToInferenceSystem(IndividualP individual);

};
typedef boost::shared_ptr<FTSEvalOp> FTSEvalOpP;


#endif //FUZZY_TIME_SERIES_FTSEVALOP_H
