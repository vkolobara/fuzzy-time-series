//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include <FIS/inference/Rule.h>
#include <FIS/inference/InferenceSystem.h>
#include "FTSEvalOp.h"
#include <cmath>

FitnessP FTSEvalOp::evaluate(IndividualP individual) {
    FitnessP fitness (new FitnessMin);

    double fitnessVal = 0.0;

    auto fis = genotypeToInferenceSystem(individual);

    auto input = make_shared<FuzzyInput>(variableParser->getInputNames());

    for (auto entry : dataset->dataset) {
        for (unsigned int i = 0; i<entry->values.size()-1; i++) {
            input->setValue(variableParser->getInputNames()[i], entry->values[i]);
        }

        auto outValue = fis->getConclusion(input);

        fitnessVal += pow(outValue - entry->values.back(), 2);
    }

    fitness->setValue(fitnessVal/dataset->dataset.size());

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

    this->variableParser = make_shared<VariableParser>();
    this->variableParser->parse(filePath);

    for (auto var : variableParser->getInputNames()) {
        variableNames.push_back(var);
    }

    sptr = state->getRegistry()->getEntry("data.input"); // get parameter value
    filePath = *((std::string*) sptr.get()); // convert from voidP to user defined type

    this->dataset = shared_ptr<Dataset>(Dataset::parseFile(filePath));
    this->defuzzifier = make_shared<COADefuzzifier>();

    sptr = state->getRegistry()->getEntry("fuzzy.numrules");
    this->numRules = *((uint*) sptr.get());

    return true;
}

shared_ptr<MamdaniInferenceSystem> FTSEvalOp::genotypeToInferenceSystem(IndividualP individual) {

    auto genotype = (FloatingPoint::FloatingPoint*) individual->getGenotype().get();

    // num rules
    vector<shared_ptr<Rule>> rules;

    for (unsigned int i = 0; i<numRules; i++) {
        auto outputVar = variableParser->getOuputVariable("perc_out");
        // i * num_lang_vars + (num_lang_vars - 1)
        auto termName = outputVar->getTermNames()[(int)genotype->realValue[i*2+1]];

        vector<shared_ptr<Clause>> clauses;

        // num clauses
        for (unsigned int j = 0; j<1; j++) {
            auto var = variableParser->getInputVariable(variableNames[j]);
            auto name = var->getTermNames()[genotype->realValue[i*2 + j]];
            auto term = var->getLanguageTerm(name);

            clauses.push_back(make_shared<SimpleClause>(term, var));
        }

        auto antecedent = make_shared<AndClause>(clauses);
        auto consequense = outputVar->getMeaning(termName);
        rules.push_back(make_shared<Rule>(antecedent, consequense, make_shared<Zadeh::TNorm>()));
    }

    return make_shared<MamdaniInferenceSystem>(rules, make_shared<Zadeh::SNorm>(), defuzzifier);
}
