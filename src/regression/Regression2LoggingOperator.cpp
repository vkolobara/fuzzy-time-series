//
// Created by vkolobara on 6/2/18.
//

#include "FinancialLoggingOperator.h"
#include "Regression2FTSEvalOp.h"

bool Regression2LoggingOperator::operate(StateP state) {
    auto evalOp = (Regression2FTSEvalOp*) (state->getEvalOp().get());

    if (evalOp->fileLogger) {

        auto bestIndividual = state->getPopulation()->getHof()->getBest()[0];

        auto dataset = evalOp->dataset;
        auto testDataset = evalOp->testDataset;

        evalOp->fileLogger->log(state->getGenerationNo(), bestIndividual->getFitness()->getValue(), evalOp->evaluate(bestIndividual, dataset)->getValue());
    }

    return true;
}
