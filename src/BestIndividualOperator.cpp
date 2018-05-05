//
// Created by vkolobara on 3/30/18.
//

#include "BestIndividualOperator.h"
#include "ClassifierFTSEvalOp.h"

bool BestIndividualOperator::operate(StateP state) {
    auto selOp = make_shared<SelFitnessProportionalOp>();
    selOp->initialize(state);
    auto evalOp = (ClassifierFTSEvalOp*)(state->getEvalOp().get());

    auto individuals = state->getPopulation()->getLocalDeme();

    vector<boost::shared_ptr<Individual>> selected(10);

    for (auto i=0; i<state->getPopulation()->getNoDemes(); i++) {
        selected[i] = selOp->select(*state->getPopulation()->at(i).get());
    }

    //auto selected = selOp->selectMany(*individuals, evalOp->numRules);

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
        //double conclusion = 0;
        //double bestActivation = 0;
        double sumC = 0;
        double sum = 0;
        for (auto rule : rules) {
            double activation = rule->antecedent.get()->getActivation();
            //auto consequent = static_pointer_cast<ConstantConsequent>(rule->consequent);

            auto consequent = static_pointer_cast<LinearVariableConsequent>(rule->consequent);

            sumC += activation * consequent->membership();
            sum += activation;

            //if (activation >= bestActivation) {
            //    conclusion = consequent->membership();
            //    bestActivation = activation;
            //}
        }

        if (sum <= 1e-6) sum = 1;
        auto conclusion = sumC / sum;

        cout << "REAL: " << row->values.back() << "; PRED: " << conclusion << endl;

        sumFit += evalOp->errorFunction->error(row->values.back(), conclusion);
    }

    fitnessVal = sumFit / dataset->getNumRows();

    cout << "FITNESS VAL: " << fitnessVal << endl << endl;


    return true;
}
