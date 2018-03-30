//
// Created by vkolobara on 3/30/18.
//

#include "BestIndividualOperator.h"
#include "FTSEvalOp.h"

bool BestIndividualOperator::operate(StateP state) {

    auto individual = state->getHoF()->getBest()[0];
    auto evalOp = (FTSEvalOp*) (state->getEvalOp().get());

    auto fis = evalOp->genotypeToInferenceSystem(individual);

    auto input = make_shared<FuzzyInput>(evalOp->variableParser->getInputNames());

    for (auto entry : evalOp->dataset->dataset) {
        for (unsigned int i = 0; i<entry->values.size()-1; i++) {
            input->setValue(evalOp->variableParser->getInputNames()[i], entry->values[i]);
        }

        auto outValue = fis->getConclusion(input);
        cout << "PREDICTED: " << outValue << " | REAL: " << entry->values.back() << endl;
    }


    return true;
}
