//
// Created by Vinko Kolobara on 27. 5. 2018..
//

#include <ECF/ECF.h>
#include "ANFISEvalOp.h"

int main(int argc, char **argv) {
    StateP state (new State);
    state->setEvalOp((ANFISEvalOpP) new ANFISEvalOp);

    state->initialize(argc, argv);

    state->run();

    return 0;
}
