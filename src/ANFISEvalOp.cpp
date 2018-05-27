//
// Created by Vinko Kolobara on 27. 5. 2018..
//

#include <ANFIS/membership/GaussMembershipFunction.h>
#include "ANFISEvalOp.h"

shared_ptr<ANFIS> ANFISEvalOp::genotypeToANFIS(IndividualP individual) {
    vector<shared_ptr<Rule>> rules(this->numRules);


    auto genotype = (FloatingPoint::FloatingPoint*) individual->getGenotype().get();

    vector<vector<shared_ptr<AnfisMembershipFunction>>> antecedentParams;
    vector<vector<double>> consequentParams;

    for (auto j = 0; j<this->numRules; j++) {
        vector<shared_ptr<AnfisMembershipFunction>> funcs;
        auto i = 0;
        for (; i < 2*this->numVars; i+=2) {

            auto param1 = genotype->realValue[j*(this->numVars*3+1) + i];
            auto param2 = genotype->realValue[j*(this->numVars*3+1) + i + 1];

            auto func = make_shared<GaussMembershipFunction>();

            vector<double> ps({param1, param2});

            func->updateParameters(ps);
            funcs.push_back(func);

        }

        antecedentParams.push_back(funcs);

        vector<double> params;
        for (; i < 3*this->numVars + 1; i++) {
            params.push_back(genotype->realValue[j*this->numVars*2 + i]);
        }
        consequentParams.push_back(params);

    }

    auto antecedentLayer = make_shared<AntecedentLayer>(this->numRules, make_shared<ProductTNorm>(), antecedentParams);
    auto consequentLayer = make_shared<ConsequentLayer>(consequentParams);
    auto normalizingLayer = make_shared<NormalizingLayer>();
    auto outputLayer = make_shared<OutputLayer>();

    return make_shared<ANFIS>(this->numRules, antecedentLayer, normalizingLayer, consequentLayer, outputLayer);
}

FitnessP ANFISEvalOp::evaluate(IndividualP individual) {
    shared_ptr<ANFIS> anfis = genotypeToANFIS(individual);

    FitnessP fitness(new FitnessMin);

    double fitnessVal = 0;

    for (auto i=0; i<dataset->getNumRows(); i++) {
        auto row = dataset->dataset.at(i)->values;
        vector<double> vals = vector<double>(row.begin(), row.end()-1);

        auto res = anfis->forward(vals);
        cout << res[0] << ":" << row.back() << endl;
        fitnessVal += errorFunction->error(row.back(), res[0]);
    }
    cout << endl;

    fitness->setValue(fitnessVal);
    return fitness;
}

bool ANFISEvalOp::initialize(StateP state) {

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

    sptr = state->getRegistry()->getEntry("data.input"); // get parameter value
    filePath = *((std::string*) sptr.get()); // convert from voidP to user defined type

    this->dataset = shared_ptr<Dataset>(Dataset::parseFile(filePath));

    sptr = state->getRegistry()->getEntry("fuzzy.numrules");
    this->numRules = *((uint*) sptr.get());

    this->numVars = variableParser->inputVariables.size();

    state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(this->numRules * (this->numVars * 3 + 1)));

    // NUM RULES * NUM VARS * NUM OF PARAMETERS IN LANGUAGE TERM
    //state->getGenotypes()[0]->setParameterValue(state, "dimension", (voidP) new uint(this->numRules * this->numVars * 3 + 1));

    state->getPopulation()->initialize(state);

    this->errorFunction = make_shared<MeanAbsolutePercentageError>();

    return true;
}

