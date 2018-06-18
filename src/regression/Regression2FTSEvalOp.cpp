//
// Created by vkolobara on 5/4/18.
//

#include "Regression2FTSEvalOp.h"

FitnessP Regression2FTSEvalOp::evaluate(IndividualP individual) {
    return evaluate(individual, dataset);
}

vector<shared_ptr<Rule>> Regression2FTSEvalOp::genotypeToRules(IndividualP individual) {
    vector<shared_ptr<Rule>> rules(this->numRules);

    auto genotypeConsequent = (FloatingPoint::FloatingPoint*) individual->getGenotype(0).get();

    uint currIndex = 0;
    for (auto j = 0; j<this->numRules; j++) {
        auto antecedent = new Antecedent(*new Zadeh::TNorm());
        auto var = shared_ptr<LanguageVariable>(knowledgeBase->getVariable(variableNames[0]));
        auto term = this->rules->antecedents[j];

        antecedent->addClause(*(new Clause(var, term)));


        vector<double> ws;

        vector<shared_ptr<Clause>> clauses;

        for (auto i = 0; i < this->rules->consequents[j].size(); i++) {

            auto var = variableParser->inputVariables[0];
            auto term = this->rules->consequents[j][i];

            ws.push_back(genotypeConsequent->realValue[currIndex + j]);
            clauses.push_back(make_shared<Clause>(var, term));
        }

        currIndex+=this->rules->consequents[j].size();

        auto consequent = new LinearClauseConsequent(ws, clauses);
        rules[j] = make_shared<Rule>(*antecedent, *consequent);
    }

    return rules;
}

bool Regression2FTSEvalOp::initialize(StateP state) {

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

    if (!state->getRegistry()->isModified("regression.rules")) {
        ECF_LOG_ERROR(state, "Error: no rules defined! (parameter 'regression.rules'");
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

    sptr = state->getRegistry()->getEntry("regression.rules"); // get parameter value
    filePath = *((std::string *) sptr.get()); // convert from voidP to user defined type
    this->rules = shared_ptr<RegressionRules>(RegressionRules::parseFile(filePath));

    sptr = state->getRegistry()->getEntry("fuzzy.numrules");
    this->numRules = *((uint *) sptr.get());

    this->numRules = this->rules->antecedents.size();

    this->numVars = variableParser->inputVariables.size();

    //state->getRegistry()->modifyEntry("population.demes", (voidP) new uint(this->numRules));
    //state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(this->numRules * (this->numVars)));

    state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(rules->getTotalParams()));
    state->getPopulation()->initialize(state);

    this->errorFunction = make_shared<MeanAbsolutePercentageError>();
    return true;
}

FitnessP Regression2FTSEvalOp::evaluate(IndividualP individual, shared_ptr<Dataset> dataset) {
    FitnessP fitness(new FitnessMin);

    double fitnessVal = 0.0;

    auto rules = genotypeToRules(individual);

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

        fitnessVal += this->errorFunction->error(row->values.back(), conc);
    }

    fitness->setValue(fitnessVal / dataset.get()->getNumRows());
    return fitness;
}

void Regression2FTSEvalOp::registerParameters(StateP state) {
    ClassifierFTSEvalOp::registerParameters(state);
    state->getRegistry()->registerEntry("regression.rules", (voidP) (new string("")), ECF::STRING);
}

