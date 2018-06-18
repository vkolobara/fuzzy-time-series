//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include <ECF/ECF.h>
#include "../classification/ClassifierFTSEvalOp.h"
#include "Regression2FTSEvalOp.h"

int main(int argc, char **argv) {
    StateP state (new State);
    state->setEvalOp((Regression2FTSEvalOpP) new Regression2FTSEvalOp);
    //state->addOperator((RegressionLoggingOperatorP) new RegressionLoggingOperator);

    state->initialize(argc, argv);

    state->run();

    return 0;
}
