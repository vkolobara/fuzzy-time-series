//
// Created by vkolobara on 5/4/18.
//

#include "RegressionFTSEvalOp.h"

FitnessP RegressionFTSEvalOp::evaluate(IndividualP individual) {
    FitnessP fitness(new FitnessMin);

    double fitnessVal = 0.0;

    auto rule = genotypeToRule(individual);

    for (auto row : dataset.get()->dataset) {
        for (auto i = 0; i < row->values.size() - 1; i++) {
            knowledgeBase->getVariable(variableNames[i])->value = row->values.at(i);
        }

        auto activation = rule->antecedent.get()->getActivation();
        auto consequent = static_pointer_cast<LinearVariableConsequent>(rule->consequent);

        auto conclusion = activation * consequent->membership();

        fitnessVal += this->errorFunction->error(row->values.back(), conclusion);
    }

    fitness->setValue(fitnessVal / dataset.get()->getNumRows());
    return fitness;
}

shared_ptr<Rule> RegressionFTSEvalOp::genotypeToRule(IndividualP individual) {
    auto genotype = (FloatingPoint::FloatingPoint *) individual->getGenotype().get();
    auto antecedent = new Antecedent(*new Zadeh::TNorm());
    for (auto i = 0; i < this->numVars; i++) {
        auto var = shared_ptr<LanguageVariable>(knowledgeBase->getVariable(variableNames[i]));
        auto term = (uint) genotype->realValue[i];

        antecedent->addClause(*(new Clause(var, term)));
    }

    vector<double> ws(this->numVars);

    for (auto i = 0; i < this->numVars; i++) {
        ws[i] = genotype->realValue[this->numVars + i];
    }

    auto consequent = new LinearVariableConsequent(ws, this->variableParser->inputVariables);

    return make_shared<Rule>(*antecedent, *consequent);
}

bool RegressionFTSEvalOp::initialize(StateP state) {
    if (ClassifierFTSEvalOp::initialize(state)) {

        this->numVars = variableParser.get()->inputVariables.size();
        state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(2*this->numVars));
        state->getPopulation()->initialize(state);

        this->errorFunction = make_shared<MeanAbsolutePercentageError>();
        return true;
    }

    return false;

}

