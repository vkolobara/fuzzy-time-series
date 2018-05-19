//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include <ECF/ECF.h>
#include "ClassifierFTSEvalOp.h"
#include "BestIndividualOperator.h"
#include "RegressionFTSEvalOp.h"

int main(int argc, char **argv) {
    StateP state (new State);
    state->setEvalOp((ClassifierFTSEvalOpP) new ClassifierFTSEvalOp);

    state->initialize(argc, argv);

    state->run();

    return 0;
}
