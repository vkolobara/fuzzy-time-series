//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include <FIS/inference/Rule.h>
#include <FIS/inference/InferenceSystem.h>
#include "FTSEvalOp.h"
#include "math.h"

FitnessP FTSEvalOp::evaluate(IndividualP individual) {
    FitnessP fitness (new FitnessMin);

    double fitnessVal = 0.0;

    auto fis = genotypeToInferenceSystem(individual);

    FuzzyInput* input = new FuzzyInput(variableParser->getInputNames());

    for (auto entry : dataset->dataset) {
        cout << "ENTRY: " << entry->values[0] << ", " << entry->values[1] << endl;
        for (unsigned int i = 0; i<entry->values.size()-1; i++) {
            input->setValue(variableParser->getInputNames()[i], new DomainElement({entry->values[i]}));
        }

        auto outValue = fis.getConclusion(input);

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

    voidP sptr = state->getRegistry()->getEntry("fuzzy.langvars"); // get parameter value
    string filePath = *((std::string*) sptr.get()); // convert from voidP to user defined type

    this->variableParser = new VariableParser();
    this->variableParser->parse(filePath);

    for (auto var : variableParser->getInputNames()) {
        variableNames.push_back(var);
    }

    sptr = state->getRegistry()->getEntry("data.input"); // get parameter value
    filePath = *((std::string*) sptr.get()); // convert from voidP to user defined type

    this->dataset = Dataset::parseFile(filePath);
    this->defuzzifier = new COADefuzzifier();

    return true;
}

MamdaniInferenceSystem FTSEvalOp::genotypeToInferenceSystem(IndividualP individual) {

    auto genotype = (FloatingPoint::FloatingPoint*) individual->getGenotype().get();

    // num rules
    vector<Rule> rules(5);

    for (unsigned int i = 0; i<5; i++) {
        auto outputVar = variableParser->getOuputVariable("perc_out");
        // i * num_lang_vars + (num_lang_vars - 1)
        auto termName = outputVar->getTermNames()[(int)genotype->realValue[i*2+1]];

        vector<Clause*> clauses;

        // num rules
        for (unsigned int j = 0; j<1; j++) {
            auto var = variableParser->getInputVariable(variableNames[j]);
            auto name = var->getTermNames()[genotype->realValue[i*2 + j]];
            auto term = var->getLanguageTerm(name);

            clauses.push_back(new SimpleClause(term, var));
        }

        Clause* antecedent = new AndClause(clauses);
        FuzzySet* consequense = outputVar->getMeaning(termName);
        rules[i] = Rule(antecedent, consequense, new Zadeh::TNorm());
    }

    return MamdaniInferenceSystem(rules, new Zadeh::SNorm(), defuzzifier);
}
