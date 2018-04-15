//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include "FTSEvalOp.h"

FitnessP FTSEvalOp::evaluate(IndividualP individual) {
    FitnessP fitness (new FitnessMin);

    double fitnessVal = 0.0;

    auto inferenceSystem = genotypeToInferenceSystem(individual);

    for (auto row : dataset->dataset) {
        for (auto i = 0; i < row->values.size()-1; i++) {
            inferenceSystem->getVariable(i)->value = row->values.at(i);
        }

        double conclusion = inferenceSystem->getConclusion(this->defuzzifier);

        fitnessVal += pow(conclusion - row->values.back(), 2);
    }

    delete inferenceSystem;

    fitness->setValue(fitnessVal/dataset->getNumRows());
    return fitness;
}

void FTSEvalOp::registerParameters(StateP state) {
    //register entry langvar for file with Language Variable definition
    state->getRegistry()->registerEntry("fuzzy.langvars", (voidP) (new std::string), ECF::STRING);

    //register entry data.input for the data file
    state->getRegistry()->registerEntry("data.input", (voidP) (new std::string), ECF::STRING);

    //register entry fuzzy.numrules for the number of rules
    state->getRegistry()->registerEntry("fuzzy.numrules", (voidP) (new uint(1)), ECF::UINT);
}

bool FTSEvalOp::initialize(StateP state) {

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

    this->variableParser = new VariableParser();
    this->variableParser->parse(buffer.str());

    sptr = state->getRegistry()->getEntry("data.input"); // get parameter value
    filePath = *((std::string*) sptr.get()); // convert from voidP to user defined type

    this->dataset = Dataset::parseFile(filePath);
    this->defuzzifier = new COADefuzzifier();

    sptr = state->getRegistry()->getEntry("fuzzy.numrules");
    this->numRules = *((uint*) sptr.get());

    // Always expecting to receive one output variable
    this->numVars = variableParser->inputVariables.size() + 1;

    //state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(this->numRules * this->numVars));

    return true;
}

InferenceSystem* FTSEvalOp::genotypeToInferenceSystem(IndividualP individual) {
    auto genotype = (FloatingPoint::FloatingPoint*) individual->getGenotype().get();

    auto inferenceSystem = new InferenceSystem(new Zadeh::SNorm());

    vector<LanguageVariable*> variableCopies;

    for (auto i = 0; i < this->numVars-1; i++) {
        auto var = this->variableParser->inputVariables.at(i)->clone();
        variableCopies.push_back(var);
        inferenceSystem->addVariable(var);
    }
    auto var = this->variableParser->outputVariables.front()->clone();
    variableCopies.push_back(var);
    inferenceSystem->addVariable(var);

    for (auto i = 0; i<this->numRules; i++) {
        auto antecedent = new Antecedent(new Zadeh::TNorm());

        for (auto j =0; j<this->numVars-1; j++) {
            auto var = variableCopies.at(j);
            auto term = var->terms.at((uint)genotype->realValue[i * this->numRules + j]);

            antecedent->addClause(new Clause(var, term));
        }

        auto var = variableCopies.back();
        auto term = var->terms.at((uint)genotype->realValue[(i+1)*(this->numRules)]);

        auto consequent = new Clause(var, term);

        auto rule = new Rule(antecedent, consequent);

        inferenceSystem->addRule(rule);
    }

    return inferenceSystem;

}
