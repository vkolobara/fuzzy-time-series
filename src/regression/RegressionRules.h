//
// Created by vkolobara on 6/18/18.
//

#ifndef FUZZY_TIME_SERIES_REGRESSIONRULES_H
#define FUZZY_TIME_SERIES_REGRESSIONRULES_H


#include <ECF/ECF.h>

struct RegressionRules {

    vector<uint> antecedents;
    vector<vector<uint>> consequents;

    uint getTotalParams();

    RegressionRules(const vector<uint> &antecedents, const vector<vector<uint>> &consequents);

    static RegressionRules* parseFile(string filePath);


};


#endif //FUZZY_TIME_SERIES_REGRESSIONRULES_H
