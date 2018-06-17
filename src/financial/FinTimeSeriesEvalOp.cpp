//
// Created by Vinko Kolobara on 18. 5. 2018..
//

#include "FinTimeSeriesEvalOp.h"

FitnessP FinTimeSeriesEvalOp::evaluate(IndividualP individual) {
    return evaluate(individual, this->dataset, 4);
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
    if (!state->getRegistry()->isModified("data.test")) {
        ECF_LOG_ERROR(state, "Error: no test data file defined! (parameter 'data.test'");
        return false;
    }

    //get registered entry and parse the file with LanguageVariablesParser
    if (!state->getRegistry()->isModified("fuzzy.numrules")) {
        ECF_LOG_ERROR(state, "Error: no number of rules defined! (parameter 'fuzzy.numrules'");
        return false;
    }

    //get registered entry and parse the file with LanguageVariablesParser
    if(!state->getRegistry()->isModified("data.balance")) {
        ECF_LOG_ERROR(state, "Error: no start balance specified! (parameter 'data.balance'");
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

    startBalance = *((double *) state->getRegistry()->getEntry("data.balance").get());
    threshold = *((double *) state->getRegistry()->getEntry("fuzzy.threshold").get());

    this->dataset = shared_ptr<Dataset>(Dataset::parseFile(filePath));

    sptr = state->getRegistry()->getEntry("data.test"); // get parameter value
    filePath = *((std::string *) sptr.get()); // convert from voidP to user defined type
    this->testDataset = shared_ptr<Dataset>(Dataset::parseFile(filePath));



    sptr = state->getRegistry()->getEntry("fuzzy.numrules");
    this->numRules = *((uint *) sptr.get());

    this->numVars = variableParser->inputVariables.size();

    // Antecedent (all variables)
    state->getGenotypes()[0]->setParameterValue(state, "dimension", (voidP) new uint(this->numVars * this->numRules));

    // Consequent (2 weights, one for buy, one for sell)
    state->getGenotypes()[1]->setParameterValue(state, "dimension", (voidP) new uint(2 * this->numRules));

    //state->getRegistry()->modifyEntry("population.demes", (voidP) new uint(this->numRules));
    //state->getRegistry()->modifyEntry("FloatingPoint.dimension", (voidP) new uint(this->numRules * (this->numVars)));

    state->getPopulation()->initialize(state);

    this->errorFunction = make_shared<MeanAbsolutePercentageError>();
    return true;
}

/*
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
*/

vector<shared_ptr<Rule>> FinTimeSeriesEvalOp::genotypeToRules(IndividualP individual) {

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
        auto consequent = new MultipleConstantConsequent({-1, 1}, {genotypeConsequent->realValue[2*j], genotypeConsequent->realValue[2*j+1]});
        rules[j] = make_shared<Rule>(*antecedent, *consequent);
    }

    return rules;
}

FitnessP FinTimeSeriesEvalOp::evaluate(IndividualP individual, shared_ptr<Dataset> dataset, int numSplits) {
    FitnessP fitness(new FitnessMax);
    const double START_BALANCE = startBalance;

    auto rules = genotypeToRules(individual);

    double buySum = 0;
    unsigned int buyCount = 0;
    double sellSum = 0;
    unsigned int sellCount = 0;

    double balance = START_BALANCE;

    vector<unsigned int> timeBought;
    vector<unsigned int> timeSold;

    unsigned int shortPosition = 0;
    unsigned int longPosition = 0;

    this->counter = (this->counter+1) % (numSplits * 5000);
    int size = dataset->getNumRows() / numSplits;
    auto currentSplit = this->counter/5000;

    for (auto i=size * currentSplit; i < size*(currentSplit+1) && i < dataset->getNumRows(); i++) {
        auto row = dataset->dataset.at(i);
        for (auto i = 0; i < row->values.size() - 1; i++) {
            knowledgeBase->getVariable(variableNames[i])->value = row->values.at(i);
        }

        double conclusionBuy = 0;
        double conclusionSell = 0;

        unsigned int count = 0;

        for (auto rule : rules) {
            auto activation = rule->antecedent->getActivation();

            auto consequent = static_pointer_cast<MultipleConstantConsequent>(rule->consequent);

            auto sellFactor = activation * consequent->weights[0];
            auto buyFactor = activation * consequent->weights[1];

            count+=activation;
            conclusionBuy += buyFactor;
            conclusionSell += sellFactor;

        }

        if (count == 0) count = 1;

        conclusionBuy /= count;
        conclusionSell /= count;

        //cout << "BUY: " << conclusionBuy << "; SELL: " << conclusionSell << endl;

        if (conclusionBuy >= conclusionSell) {
            if (conclusionBuy > this->threshold && balance > row->values.back()) {
                timeBought.push_back(i);
                if (shortPosition > 0) {
                    shortPosition--;
                    balance -= row->values.back()*1.5;
                } else {
                    longPosition++;
                    balance -= row->values.back();
                }
                balance -= 0.002*row->values.back();
            }
        } else {
            if (conclusionSell > this->threshold) {
                timeSold.push_back(i);
                balance += row->values.back();
                if (longPosition > 0) {
                    longPosition--;
                } else {
                    shortPosition++;
                }
                balance -= 0.002 * row->values.back();
            }
        }

    }

    //if (buyCount == 0) buyCount = 1;
    //if (sellCount == 0) sellCount = 1;

    //auto buyValue = buySum / buyCount;
    //auto sellValue = sellSum / sellCount;

    //if (buyValue < 1e-7) fitness->setValue(0);
    //else fitness->setValue(sellValue / buyValue);

    //cout << "BUY: " << buyValue << "; SELL: " << sellValue << endl;


    balance += dataset->dataset.back()->values.back() * longPosition;
    balance -= dataset->dataset.back()->values.back() * shortPosition * 1.5;
    /*
    if (balance/START_BALANCE > 1) {
        cout << "BALANCE: " << balance << endl <<  "BOUGHT: ";
        for (auto i = timeBought.begin(); i != timeBought.end(); ++i)
            std::cout << *i << ' ';

        cout << endl << "SOLD: ";
        for (auto i = timeSold.begin(); i != timeSold.end(); ++i)
            std::cout << *i << ' ';

        cout << endl << endl;
    }
     */

    fitness->setValue(balance/START_BALANCE);

    return fitness;
}

void FinTimeSeriesEvalOp::registerParameters(StateP state) {
    ClassifierFTSEvalOp::registerParameters(state);
    state->getRegistry()->registerEntry("data.balance", (voidP) (new double(1)), ECF::DOUBLE);
    state->getRegistry()->registerEntry("fuzzy.threshold", (voidP) (new double(0.8)), ECF::DOUBLE);

}
