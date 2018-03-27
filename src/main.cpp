//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include <ECF/ECF.h>
#include "FTSEvalOp.h"

int main(int argc, char **argv) {
    StateP state (new State);

    state->setEvalOp(new FTSEvalOp);

    state->initialize(argc, argv);

    state->run();

    return 0;
}