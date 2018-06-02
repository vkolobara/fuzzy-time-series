//
// Created by vkolobara on 3/30/18.
//

#include "RegressionLoggingOperator.h"
#include "../classification/ClassifierFTSEvalOp.h"
#include "RegressionFTSEvalOp.h"

bool RegressionLoggingOperator::operate(StateP state) {

    auto evalOp = (RegressionFTSEvalOp*)(state->getEvalOp().get());
    if (evalOp->fileLogger)
        evalOp->fileLogger->log(state->getGenerationNo(), evaluate(state, evalOp->dataset), evaluate(state, evalOp->testDataset));

    return true;
}

double RegressionLoggingOperator::evaluate(StateP state, shared_ptr<Dataset> dataset) {

    auto evalOp = (RegressionFTSEvalOp*)(state->getEvalOp().get());

    auto selected = state->getHoF()->getBest();

    vector<shared_ptr<Rule>> rules;

    auto knowledgeBase = evalOp->knowledgeBase;
    auto variableNames = evalOp->variableNames;

    for (auto sel : selected) {
        rules.push_back(shared_ptr<Rule>(evalOp->genotypeToRule(sel)));
    }

    double sumFit = 0;
    for (auto row : dataset->dataset) {
        for (auto i = 0; i < row->values.size()-1; i++) {
            knowledgeBase->getVariable(variableNames[i])->value = row->values.at(i);
        }

        double sumC = 0;
        double sum = 0;
        for (auto rule : rules) {
            double activation = rule->antecedent.get()->getActivation();

            auto consequent = static_pointer_cast<LinearVariableConsequent>(rule->consequent);

            sumC += activation * consequent->membership();
            sum += activation;

        }

        if (sum <= 1e-6) sum = 1;
        auto conclusion = sumC / sum;

        sumFit += evalOp->errorFunction->error(row->values.back(), conclusion);
    }

    return sumFit / dataset->getNumRows();

}
