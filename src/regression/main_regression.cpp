//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include <ECF/ECF.h>
#include "../classification/ClassifierFTSEvalOp.h"
#include "RegressionFTSEvalOp.h"
#include "RegressionLoggingOperator.h"

int main(int argc, char **argv) {
    StateP state (new State);
    state->setEvalOp((RegressionFTSEvalOpP) new RegressionFTSEvalOp);
    state->addOperator((RegressionLoggingOperatorP) new RegressionLoggingOperator);

    state->initialize(argc, argv);

    state->run();

    return 0;
}
