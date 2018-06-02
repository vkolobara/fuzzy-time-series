//
// Created by vkolobara on 6/2/18.
//

#include "ClassificationLoggingOperator.h"
#include "ClassifierFTSEvalOp.h"

bool ClassificationLoggingOperator::operate(StateP state) {
    auto evalOp = (ClassifierFTSEvalOp *) (state->getEvalOp().get());

    if (evalOp->fileLogger) {

        auto bestIndividual = state->getPopulation()->getHof()->getBest()[0];

        auto dataset = evalOp->dataset;
        auto testDataset = evalOp->testDataset;

        evalOp->fileLogger->log(state->getGenerationNo(), bestIndividual->getFitness()->getValue(), evalOp->evaluate(bestIndividual, testDataset)->getValue());
    }

    return true;
}
