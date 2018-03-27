//
// Created by Vinko Kolobara on 16. 3. 2018..
//

#ifndef FUZZY_TIME_SERIES_DATASET_H
#define FUZZY_TIME_SERIES_DATASET_H

#include <vector>
#include <string>
#include "DataRow.h"

using namespace std;

struct Dataset {
    vector<DataRow*> dataset;

    explicit Dataset(const vector<DataRow*> &dataset);

    static Dataset* parseFile(string filePath);
};


#endif //FUZZY_TIME_SERIES_DATASET_H
