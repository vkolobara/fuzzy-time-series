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

        evalOp->fileLogger->log(state->getGenerationNo(), bestIndividual->getFitness()->getValue(), evalOp->evaluate(bestIndividual, testDataset, 1)->getValue());
    }

    return true;
}
