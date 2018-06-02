//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include "ClassifierFTSEvalOp.h"

FitnessP ClassifierFTSEvalOp::evaluate(IndividualP individual) {
    return evaluate(individual, dataset);
}

void ClassifierFTSEvalOp::registerParameters(StateP state) {
    //register entry langvar for file with Language Variable definition
    state->getRegistry()->registerEntry("fuzzy.langvars", (voidP) (new std::string), ECF::STRING);

    //register entry data.input for the data file
    state->getRegistry()->registerEntry("data.input", (voidP) (new std::string), ECF::STRING);

    //register entry fuzzy.numrules for the number of rules
    state->getRegistry()->registerEntry("fuzzy.numrules", (voidP) (new uint(1)), ECF::UINT);

    state->getRegistry()->registerEntry("data.test", (voidP) (new std::string), ECF::STRING);

    state->getRegistry()->registerEntry("operator.logfile", (voidP) (new std::string), ECF::STRING);
}

bool ClassifierFTSEvalOp::initialize(StateP state) {

    //get registered entry and parse the file with LanguageVariablesParser
    if(!state->getRegistry()->isModified("fuzzy.langvars")) {
        ECF_LOG_ERROR(state, "Error: no input file defined for language variables! (parameter 'fuzzy.langvars'");
        return false;
    }

    //get registered entry and parse the file with LanguageVariablesParser
    if(!state->getRegistry()->isModified("data.input")) {
        ECF_LOG_ERROR(state, "Error: no input data file defined! (parameter 'data.input'");
        return false;
    }

    //get registered entry and parse the file with LanguageVariablesParser
    if (!state->getRegistry()->isModified("data.test")) {
        ECF_LOG_ERROR(state, "Error: no test data file defined! (parameter 'data.test'");
        return false;
    }

    //get registered entry and parse the file with LanguageVariablesParser
    if(!state->getRegistry()->isModified("fuzzy.numrules")) {
        ECF_LOG_ERROR(state, "Error: no number of rules defined! (parameter 'fuzzy.numrules'");
        return false;
    }

    if (state->getRegistry()->isModified("operator.logfile")) {
        voidP sptr = state->getRegistry()->getEntry("operator.logfile"); // get parameter value
        string filePath = *((std::string*) sptr.get()); // convert from voidP to user defined type
        this->fileLogger = make_shared<FileLogger>(filePath);
    }

    voidP sptr = state->getRegistry()->getEntry("fuzzy.langvars"); // get parameter value
    string filePath = *((std::string*) sptr.get()); // convert from voidP to user defined type

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
    filePath = *((std::string*) sptr.get()); // convert from voidP to user defined type

    this->dataset = shared_ptr<Dataset>(Dataset::parseFile(filePath));

    sptr = state->getRegistry()->getEntry("data.test"); // get parameter value
    filePath = *((std::string *) sptr.get()); // convert from voidP to user defined type
    this->testDataset = shared_ptr<Dataset>(Dataset::parseFile(filePath));

    sptr = state->getRegistry()->getEntry("fuzzy.numrules");
    this->numRules = *((uint*) sptr.get());

    this->numVars = variableParser->inputVariables.size();

    //state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(this->numRules * (this->numVars)));

    state->getGenotypes()[0]->setParameterValue(state, "dimension", (voidP) new uint(this->numRules * this->numVars));
    state->getGenotypes()[1]->setParameterValue(state, "dimension", (voidP) new uint(this->numRules));

    state->getPopulation()->initialize(state);

    this->errorFunction = make_shared<MisclassificationError>();

    return true;
}

shared_ptr<Rule> ClassifierFTSEvalOp::genotypeToRule(IndividualP individual) {

    auto genotypeAntecedent = (FloatingPoint::FloatingPoint*) individual->getGenotype(0).get();
    auto genotypeConsequent = (FloatingPoint::FloatingPoint*) individual->getGenotype(1).get();

    auto antecedent = new Antecedent(*new Zadeh::TNorm());
    for (auto i = 0; i<this->numVars; i++) {
        auto var = shared_ptr<LanguageVariable>(knowledgeBase->getVariable(variableNames[i]));
        auto term = (uint)genotypeAntecedent->realValue[i];

        antecedent->addClause(*(new Clause(var, term)));
    }
    auto val = (uint)genotypeConsequent->realValue.back();

    auto consequent = new ConstantConsequent(val);

    return make_shared<Rule>(*antecedent, *consequent);
}


vector<shared_ptr<Rule>> ClassifierFTSEvalOp::genotypeToRules(IndividualP individual) {

    vector<shared_ptr<Rule>> rules(this->numRules);

    auto genotypeAntecedent = (FloatingPoint::FloatingPoint*) individual->getGenotype(0).get();
    auto genotypeConsequent = (FloatingPoint::FloatingPoint*) individual->getGenotype(1).get();

    for (auto j = 0; j<this->numRules; j++) {
        auto antecedent = new Antecedent(*new Zadeh::TNorm());
        for (auto i = 0; i < this->numVars; i++) {

            auto var = shared_ptr<LanguageVariable>(knowledgeBase->getVariable(variableNames[i]));
            auto term = (uint) genotypeAntecedent->realValue[j*this->numVars + i];

            antecedent->addClause(*(new Clause(var, term)));
        }
        auto val = (uint) genotypeConsequent->realValue[j];

        auto consequent = new ConstantConsequent(val);
        rules[j] = make_shared<Rule>(*antecedent, *consequent);
    }

    return rules;
}

FitnessP ClassifierFTSEvalOp::evaluate(IndividualP individual, shared_ptr<Dataset> dataset) {
    FitnessP fitness (new FitnessMin);

    double fitnessVal = 0.0;

    auto rules = genotypeToRules(individual);

    for (auto row : dataset->dataset) {

        for (auto i = 0; i < row->values.size()-1; i++) {
            knowledgeBase->getVariable(variableNames[i])->value = row->values.at(i);
        }

        double conclusion = 0;
        double bestActivation = 0;

        for (auto rule : rules) {

            double activation = rule->antecedent->getActivation();

            if (activation >= bestActivation) {
                conclusion = rule->consequent->membership();
                bestActivation = activation;
            }
        }
        fitnessVal += this->errorFunction->error(row->values.back(), conclusion);
    }

    fitness->setValue(fitnessVal/dataset->getNumRows());
    return fitness;
}
