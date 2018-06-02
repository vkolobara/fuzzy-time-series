//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#ifndef FUZZY_TIME_SERIES_FTSEVALOP_H
#define FUZZY_TIME_SERIES_FTSEVALOP_H

#include <ECF/ECF.h>
#include <FIS/rule/InferenceSystem.h>
#include "FIS/parser/VariableParser.h"
#include "../dataset/Dataset.h"
#include "../ErrorFunction.h"
#include "../FileLogger.h"

class ClassifierFTSEvalOp : public EvaluateOp {
public:
    shared_ptr<VariableParser> variableParser;
    shared_ptr<Dataset> dataset;
    shared_ptr<Dataset> testDataset;
    vector<std::string> variableNames;
    shared_ptr<KnowledgeBase> knowledgeBase;

    shared_ptr<FileLogger> fileLogger;

    uint numRules;
    uint numVars;

    shared_ptr<ErrorFunction> errorFunction;

public:
    FitnessP evaluate(IndividualP individual) override;

    virtual FitnessP evaluate(IndividualP individual, shared_ptr<Dataset> dataset);
    void registerParameters(StateP) override;
    bool initialize(StateP) override;
    virtual shared_ptr<Rule> genotypeToRule(IndividualP individual);

    virtual vector<shared_ptr<Rule>> genotypeToRules(IndividualP individual);
};
typedef boost::shared_ptr<ClassifierFTSEvalOp> ClassifierFTSEvalOpP;


#endif //FUZZY_TIME_SERIES_FTSEVALOP_H
