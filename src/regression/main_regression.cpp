//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include <ECF/ECF.h>
#include "../classification/ClassifierFTSEvalOp.h"
#include "Regression2FTSEvalOp.h"
#include "Regression2LoggingOperator.h"

int main(int argc, char **argv) {
    StateP state (new State);
    state->setEvalOp((Regression2FTSEvalOpP) new Regression2FTSEvalOp);
    state->addOperator((Regression2LoggingOperatorP) new Regression2LoggingOperator);

    state->initialize(argc, argv);

    state->run();

    return 0;
}
