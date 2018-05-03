//
// Created by vkolobara on 3/30/18.
//

#include "BestIndividualOperator.h"
#include "RegressionFTSEvalOp.h"

bool BestIndividualOperator::operate(StateP state) {
    auto selOp = make_shared<SelFitnessProportionalOp>();
    selOp->initialize(state);
    auto evalOp = (RegressionFTSEvalOp*)(state->getEvalOp().get());

    auto individuals = state->getPopulation()->getLocalDeme();

    auto selected = selOp->selectMany(*individuals, evalOp->numRules);

    vector<shared_ptr<Rule>> rules;
    auto dataset = evalOp->dataset;
    auto knowledgeBase = evalOp->knowledgeBase;
    auto variableNames = evalOp->variableNames;

    double fitnessVal = 0;
    for (auto sel : selected) {
        rules.push_back(shared_ptr<Rule>(evalOp->genotypeToRule(sel)));
    }

    double sumFit = 0;
    for (auto row : dataset->dataset) {
        for (auto i = 0; i < row->values.size()-1; i++) {
            knowledgeBase->getVariable(variableNames[i])->value = row->values.at(i);
        }
        double sumConclusion = 0;
        auto conseq = static_pointer_cast<FuzzyConsequent>(rules[0]->consequent);
        double min = conseq->clause->languageVariable->min;
        double max = conseq->clause->languageVariable->max;
        double step = conseq->clause->languageVariable->step;
        double sumC = 0;
        for (auto rule : rules) {
            double activation = rule->antecedent.get()->getActivation();
            auto consequent = static_pointer_cast<FuzzyConsequent>(rule->consequent);
            auto term = consequent->clause->languageVariable->getTerm(consequent->clause->termIndex);

            double sumX = 0;
            double sum = 0;

            for (auto i = min; i<=max; i+=step) {
                sumX += i * term->membership(i);
                sum += term->membership(i);
            }

            if (sum <= 1e-6) sum = 1;
            sumConclusion += activation * sumX/sum;
            sumC += activation;
        }

        if (sumC <= 1e-6) sumC = 1;
        auto conclusion = sumConclusion / sumC + 1;

        cout << "REAL: " << row->values.back() << "; PRED: " << conclusion << endl;

        sumFit += abs((conclusion - row->values.back()) / row->values.back());
    }

    fitnessVal = sumFit / dataset->getNumRows();

    cout << "FITNESS VAL: " << fitnessVal << endl << endl;


    return true;
}
