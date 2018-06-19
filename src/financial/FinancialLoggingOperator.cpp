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
        auto currentSplit = evalOp->counter / 10000 % numSplits;
        double testVal;
        if (state->getGenerationNo() > 2998) {
            testVal = evalOp->evaluate(bestIndividual, dataset, 0, 1, false)->getValue();
        } else {
            testVal = evalOp->evaluate(bestIndividual, dataset, currentSplit, numSplits, false)->getValue();
        }
        evalOp->fileLogger->log(state->getGenerationNo(), bestIndividual->getFitness()->getValue(),testVal);
    }

    return true;
}
