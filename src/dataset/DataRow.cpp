//
// Created by Vinko Kolobara on 16. 3. 2018..
//

#include "DataRow.h"
#include <FIS/parser/Helper.h>
#include <iostream>

DataRow::DataRow(const std::vector<double> &values) : values(values) {}

DataRow* DataRow::parseLine(std::string line) {
    vector<string> lineSplit = splitString(std::move(line), ",");

    vector<double> elements(lineSplit.size());

    for (unsigned int i = 0; i<elements.size(); i++) {
        elements[i] = stod(lineSplit[i]);
    }

    return new DataRow(elements);
}

unsigned int DataRow::getNumColumns() {
    return values.size();
}
