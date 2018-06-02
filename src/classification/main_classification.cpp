//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include <ECF/ECF.h>
#include "ClassifierFTSEvalOp.h"
#include "../regression/RegressionLoggingOperator.h"
#include "../regression/RegressionFTSEvalOp.h"
#include "ClassificationLoggingOperator.h"

int main(int argc, char **argv) {
    StateP state (new State);
    state->addOperator((ClassificationLoggingOperatorP) new ClassificationLoggingOperator);
    state->setEvalOp((ClassifierFTSEvalOpP) new ClassifierFTSEvalOp);

    state->initialize(argc, argv);

    state->run();

    return 0;
}
