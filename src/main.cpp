//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include <ECF/ECF.h>
#include "FTSEvalOp.h"
#include "BestIndividualOperator.h"

int main(int argc, char **argv) {
    StateP state (new State);
    state->setEvalOp((FTSEvalOpP) new FTSEvalOp);
    state->addOperator((BestIndividualOperatorP) new BestIndividualOperator);

    state->initialize(argc, argv);

    state->run();

    return 0;
}
