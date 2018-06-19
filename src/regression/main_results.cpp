//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include <ECF/ECF.h>
#include "../classification/ClassifierFTSEvalOp.h"
#include "Regression2FTSEvalOp.h"

int main(int argc, char **argv) {
    StateP state (new State);
    state->setEvalOp((Regression2FTSEvalOpP) new Regression2FTSEvalOp);

    state->initialize(argc, argv);

    auto individual = state->getHoF()->getBest().back();

    auto evalOp = (Regression2FTSEvalOp*) state->getEvalOp().get();
    auto rules = evalOp->genotypeToRules(individual);

    auto dataset = evalOp->dataset;
    auto knowledgeBase = evalOp->knowledgeBase;
    auto variableNames = evalOp->variableNames;

    uint j = 0;

    for (auto row : dataset.get()->dataset) {
        for (auto i = 0; i < row->values.size() - 1; i++) {
            knowledgeBase->getVariable(variableNames[i])->value = row->values.at(i);
        }

        auto conc = 0.0;
        auto bestAct = 0.0;

        for (auto rule : rules) {

            auto activation = rule->antecedent.get()->getActivation();
            auto consequent = static_pointer_cast<LinearClauseConsequent>(rule->consequent);

            auto conclusion = consequent->membership();
            if (activation >= bestAct) {
                bestAct = activation;
                conc = conclusion/consequent->w.size();
            }
        }

        evalOp->fileLogger->log(j++, conc, row->values.back());
    }

    return 0;
}
