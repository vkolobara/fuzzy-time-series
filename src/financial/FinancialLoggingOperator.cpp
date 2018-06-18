//
// Created by vkolobara on 6/2/18.
//

#include "FinancialLoggingOperator.h"
#include "FinTimeSeriesEvalOp.h"

bool FinancialLoggingOperator::operate(StateP state) {
    auto evalOp = (FinTimeSeriesEvalOp*) (state->getEvalOp().get());

    if (evalOp->fileLogger) {

        auto bestIndividual = state->getPopulation()->getHof()->getBest()[0];

        auto dataset = evalOp->dataset;
        auto testDataset = evalOp->testDataset;

        auto numSplits = 5;
        auto currentSplit = evalOp->counter/60000 % numSplits;

        evalOp->fileLogger->log(state->getGenerationNo(), bestIndividual->getFitness()->getValue(), evalOp->evaluate(bestIndividual, dataset, currentSplit, numSplits, false)->getValue());
    }

    return true;
}
