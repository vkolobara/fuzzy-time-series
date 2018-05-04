//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include "ClassifierFTSEvalOp.h"
#include <memory>

FitnessP ClassifierFTSEvalOp::evaluate(IndividualP individual) {
    FitnessP fitness (new FitnessMin);

    double fitnessVal = 0.0;

    auto rule = genotypeToRule(individual);

    for (auto row : dataset.get()->dataset) {
        for (auto i = 0; i < row->values.size()-1; i++) {
           knowledgeBase->getVariable(variableNames[i])->value = row->values.at(i);
        }

        double activation = rule->antecedent.get()->getActivation();
        auto consequent = static_pointer_cast<FuzzyConsequent>(rule->consequent);
        auto term = consequent->clause->languageVariable->getTerm(consequent->clause->termIndex);

        double sumX = 0;
        double sum = 0;

        double min = consequent->clause->languageVariable->min;
        double max = consequent->clause->languageVariable->max;
        double step = consequent->clause->languageVariable->step;


        for (auto i = min; i<=max; i+=step) {
            sumX += i * activation * term->membership(i);
            sum += activation * term->membership(i);
        }

        if (sum <= 1e-6) sum = 1;
        double conclusion = sumX/sum;

        fitnessVal += abs((conclusion - row->values.back())/row->values.back());
    }

    fitness->setValue(fitnessVal/dataset.get()->getNumRows());
    return fitness;
}

void ClassifierFTSEvalOp::registerParameters(StateP state) {
    //register entry langvar for file with Language Variable definition
    state->getRegistry()->registerEntry("fuzzy.langvars", (voidP) (new std::string), ECF::STRING);

    //register entry data.input for the data file
    state->getRegistry()->registerEntry("data.input", (voidP) (new std::string), ECF::STRING);

    //register entry fuzzy.numrules for the number of rules
    state->getRegistry()->registerEntry("fuzzy.numrules", (voidP) (new uint(1)), ECF::UINT);
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
    if(!state->getRegistry()->isModified("fuzzy.numrules")) {
        ECF_LOG_ERROR(state, "Error: no number of rules defined! (parameter 'fuzzy.numrules'");
        return false;
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
    for (auto var : this->variableParser->outputVariables) {
        variableNames.push_back(var->name);
        knowledgeBase->variables[var->name] = var;
    }

    sptr = state->getRegistry()->getEntry("data.input"); // get parameter value
    filePath = *((std::string*) sptr.get()); // convert from voidP to user defined type

    this->dataset = shared_ptr<Dataset>(Dataset::parseFile(filePath));

    sptr = state->getRegistry()->getEntry("fuzzy.numrules");
    this->numRules = *((uint*) sptr.get());

    this->numVars = variableParser.get()->inputVariables.size()+1;

    state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(this->numVars));
    state->getPopulation()->initialize(state);

    return true;
}

shared_ptr<Rule> ClassifierFTSEvalOp::genotypeToRule(IndividualP individual) {
    auto genotype = (FloatingPoint::FloatingPoint*) individual->getGenotype().get();
    auto antecedent = new Antecedent(*new Zadeh::TNorm());
    for (auto i =0; i<this->numVars-1; i++) {
        auto var = shared_ptr<LanguageVariable>(knowledgeBase->getVariable(variableNames[i]));
        auto term = (uint)genotype->realValue[i];

        antecedent->addClause(*(new Clause(var, term)));
    }
    auto var = shared_ptr<LanguageVariable>(knowledgeBase->getVariable(variableNames.back()));
    auto term = (uint)genotype->realValue.back();

    auto clause = new Clause(var, term);
    auto consequent = new FuzzyConsequent(*clause);

    return make_shared<Rule>(*antecedent, *consequent);
}
