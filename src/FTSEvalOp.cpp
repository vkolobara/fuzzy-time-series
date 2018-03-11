//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#include "FTSEvalOp.h"
#include "math.h"

FitnessP FTSEvalOp::evaluate(IndividualP individual) {
    FitnessP fitness (new FitnessMin);

    auto genotype = (FloatingPoint::FloatingPoint*) individual->getGenotype().get();

    genotype->realValue[0];
    auto val = abs((int)genotype->realValue[0] * 3 + (int)genotype->realValue[1] - (int)(genotype->realValue[2]*genotype->realValue[2]));

    fitness->setValue(val);

    return fitness;
}
