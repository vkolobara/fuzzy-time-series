//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#ifndef FUZZY_TIME_SERIES_FTSEVALOP_H
#define FUZZY_TIME_SERIES_FTSEVALOP_H

#include <ECF/ECF.h>

class FTSEvalOp : public EvaluateOp {
public:
    FitnessP evaluate(IndividualP individual);
};
typedef boost::shared_ptr<FTSEvalOp> FTSEvalOpP;


#endif //FUZZY_TIME_SERIES_FTSEVALOP_H
