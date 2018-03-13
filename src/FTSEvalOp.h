//
// Created by Vinko Kolobara on 11. 3. 2018..
//

#ifndef FUZZY_TIME_SERIES_FTSEVALOP_H
#define FUZZY_TIME_SERIES_FTSEVALOP_H

#include <ECF/ECF.h>
#include "FIS/parser/VariableParser.h"

class FTSEvalOp : public EvaluateOp {
private:
    VariableParser* variableParser;
public:
    FitnessP evaluate(IndividualP individual) override;
    void registerParameters(StateP) override;
    bool initialize(StateP) override;
};
typedef boost::shared_ptr<FTSEvalOp> FTSEvalOpP;


#endif //FUZZY_TIME_SERIES_FTSEVALOP_H
