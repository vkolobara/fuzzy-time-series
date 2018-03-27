//
// Created by Vinko Kolobara on 16. 3. 2018..
//

#ifndef FUZZY_TIME_SERIES_DATAENTRY_H
#define FUZZY_TIME_SERIES_DATAENTRY_H


#include <vector>
#include <string>

class DataRow {
public:
    std::vector<double> values;
    explicit DataRow(const std::vector<double> &values);
    static DataRow* parseLine(std::string line);
};


#endif //FUZZY_TIME_SERIES_DATAENTRY_H
