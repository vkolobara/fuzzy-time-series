//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#ifndef FUZZY_TIME_SERIES_FTSEVALOP_H
#define FUZZY_TIME_SERIES_FTSEVALOP_H

#include <ECF/ECF.h>
#include <FIS/rule/InferenceSystem.h>
#include "FIS/parser/VariableParser.h"
#include "dataset/Dataset.h"

class RegressionFTSEvalOp : public EvaluateOp {
public:
    shared_ptr<VariableParser> variableParser;
    shared_ptr<Dataset> dataset;
    vector<std::string> variableNames;
    shared_ptr<KnowledgeBase> knowledgeBase;

    uint numRules;
    uint numVars;

public:
    FitnessP evaluate(IndividualP individual) override;
    void registerParameters(StateP) override;
    bool initialize(StateP) override;
    shared_ptr<InferenceSystem> genotypeToInferenceSystem(IndividualP individual);
    shared_ptr<Rule> genotypeToRule(IndividualP individual);

};
typedef boost::shared_ptr<RegressionFTSEvalOp> RegressionFTSEvalOpP;


#endif //FUZZY_TIME_SERIES_FTSEVALOP_H
