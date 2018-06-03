//
// Created by vkolobara on 5/4/18.
//

#include "RegressionFTSEvalOp.h"

FitnessP RegressionFTSEvalOp::evaluate(IndividualP individual) {
    return evaluate(individual, dataset);
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
    if (!state->getRegistry()->isModified("data.test")) {
        ECF_LOG_ERROR(state, "Error: no test data file defined! (parameter 'data.test'");
        return false;
    }

    //get registered entry and parse the file with LanguageVariablesParser
    if (!state->getRegistry()->isModified("fuzzy.numrules")) {
        ECF_LOG_ERROR(state, "Error: no number of rules defined! (parameter 'fuzzy.numrules'");
        return false;
    }

    if (state->getRegistry()->isModified("operator.logfile")) {
        voidP sptr = state->getRegistry()->getEntry("operator.logfile"); // get parameter value
        string filePath = *((std::string*) sptr.get()); // convert from voidP to user defined type
        this->fileLogger = make_shared<FileLogger>(filePath);
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

    sptr = state->getRegistry()->getEntry("data.test"); // get parameter value
    filePath = *((std::string *) sptr.get()); // convert from voidP to user defined type
    this->testDataset = shared_ptr<Dataset>(Dataset::parseFile(filePath));

    sptr = state->getRegistry()->getEntry("fuzzy.numrules");
    this->numRules = *((uint *) sptr.get());

    this->numVars = variableParser->inputVariables.size();

    state->getRegistry()->modifyEntry("population.demes", (voidP) new uint(this->numRules));
    //state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(this->numRules * (this->numVars)));

    state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(2 * this->numVars));
    state->getPopulation()->initialize(state);

    this->errorFunction = make_shared<MeanAbsolutePercentageError>();
    return true;
}

FitnessP RegressionFTSEvalOp::evaluate(IndividualP individual, shared_ptr<Dataset> dataset) {
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
