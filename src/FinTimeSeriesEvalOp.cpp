//
// Created by Vinko Kolobara on 18. 5. 2018..
//

#include "FinTimeSeriesEvalOp.h"

FitnessP FinTimeSeriesEvalOp::evaluate(IndividualP individual) {
    FitnessP fitness(new FitnessMax);

    auto rule = genotypeToRule(individual);

    double buySum = 0;
    unsigned int buyCount = 0;
    double sellSum = 0;
    unsigned int sellCount = 0;

    for (auto row : dataset.get()->dataset) {
        for (auto i = 0; i < row->values.size() - 1; i++) {
            knowledgeBase->getVariable(variableNames[i])->value = row->values.at(i);
        }

        auto activation = rule->antecedent->getActivation();

        auto consequent = static_pointer_cast<MultipleConstantConsequent>(rule->consequent);

        auto sellFactor = activation * consequent->weights[0];
        auto buyFactor = activation* consequent->weights[1];

        if (sellFactor > 0.2) {
            sellSum += consequent->weights[0]*row->values.back();
            sellCount++;
        }
        if (buyFactor > 0.2) {
            buySum+=consequent->weights[1]*row->values.back();
            buyCount++;
        }

    }

    if (buyCount == 0) buyCount = 1;
    if (sellCount == 0) sellCount = 1;

    auto buyValue = buySum / buyCount;
    auto sellValue = sellSum / sellCount;

    if (buyValue < 1e-7) fitness->setValue(0);
    else fitness->setValue(sellValue / buyValue);

    //cout << "BUY: " << buyValue << "; SELL: " << sellValue << endl;

    return fitness;
}

bool FinTimeSeriesEvalOp::initialize(StateP state) {

    //get registered entry and parse the file with LanguageVariablesParser
    if (!state->getRegistry()->isModified("fuzzy.langvars")) {
        ECF_LOG_ERROR(state, "Error: no input file defined for language variables! (parameter 'fuzzy.langvars'");
        return false;
    }

    //get registered entry and parse the file with LanguageVariablesParser
    if (!state->getRegistry()->isModified("data.input")) {
        ECF_LOG_ERROR(state, "Error: no input data file defined! (parameter 'data.input'");
        return false;
    }

    //get registered entry and parse the file with LanguageVariablesParser
    if (!state->getRegistry()->isModified("fuzzy.numrules")) {
        ECF_LOG_ERROR(state, "Error: no number of rules defined! (parameter 'fuzzy.numrules'");
        return false;
    }

    voidP sptr = state->getRegistry()->getEntry("fuzzy.langvars"); // get parameter value
    string filePath = *((std::string *) sptr.get()); // convert from voidP to user defined type

    std::ifstream t(filePath);
    std::stringstream buffer;
    buffer << t.rdbuf();

    this->knowledgeBase = make_shared<KnowledgeBase>();

    this->variableParser = make_shared<VariableParser>();
    this->variableParser->parse(buffer.str());

    for (auto var : this->variableParser->inputVariables) {
        variableNames.push_back(var->name);
        knowledgeBase->variables[var->name] = var;
    }

    sptr = state->getRegistry()->getEntry("data.input"); // get parameter value
    filePath = *((std::string *) sptr.get()); // convert from voidP to user defined type

    this->dataset = shared_ptr<Dataset>(Dataset::parseFile(filePath));

    sptr = state->getRegistry()->getEntry("fuzzy.numrules");
    this->numRules = *((uint *) sptr.get());

    this->numVars = variableParser->inputVariables.size();

    // Antecedent (all variables)
    state->getGenotypes()[0]->setParameterValue(state, "dimension", (voidP) new uint(this->numVars));

    // Consequent (2 weights, one for buy, one for sell)
    state->getGenotypes()[1]->setParameterValue(state, "dimension", (voidP) new uint(2));

    state->getRegistry()->modifyEntry("population.demes", (voidP) new uint(this->numRules));
    //state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(this->numRules * (this->numVars)));

    state->getPopulation()->initialize(state);

    this->errorFunction = make_shared<MeanAbsolutePercentageError>();
    return true;
}

shared_ptr <Rule> FinTimeSeriesEvalOp::genotypeToRule(IndividualP individual) {

    auto genotypeAntecedent = (FloatingPoint::FloatingPoint*) individual->getGenotype(0).get();
    auto genotypeConsequent = (FloatingPoint::FloatingPoint*) individual->getGenotype(1).get();

    auto antecedent = new Antecedent(*new Zadeh::TNorm());
    for (auto i = 0; i<this->numVars; i++) {
        auto var = shared_ptr<LanguageVariable>(knowledgeBase->getVariable(variableNames[i]));
        auto term = (uint)genotypeAntecedent->realValue[i];

        antecedent->addClause(*(new Clause(var, term)));
    }

    // -1 -> SELL
    // 1  -> BUY
    auto consequent = new MultipleConstantConsequent({-1, 1}, {genotypeConsequent->realValue[0], genotypeConsequent->realValue[1]});

    return make_shared<Rule>(*antecedent, *consequent);
}
