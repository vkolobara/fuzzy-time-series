//
// Created by Vinko Kolobara on 19. 5. 2018..
//

#include <ECF/ECF.h>
#include "FinTimeSeriesEvalOp.h"

int main(int argc, char **argv) {
    StateP state (new State);
    state->setEvalOp((FinTimeSeriesEvalOpP) new FinTimeSeriesEvalOp);

    state->initialize(argc, argv);

    state->run();

    return 0;
}