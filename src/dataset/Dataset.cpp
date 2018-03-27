//
// Created by Vinko Kolobara on 16. 3. 2018..
//

#include <fstream>
#include "Dataset.h"

Dataset* Dataset::parseFile(string filePath) {
    ifstream file(filePath);

    vector<DataRow*> rows;

    string line;

    while (getline(file, line)) {
        if (line == "") continue;
        rows.push_back(DataRow::parseLine(line));
    }

    file.close();

    return new Dataset(rows);

}

Dataset::Dataset(const vector<DataRow*> &dataset) : dataset(dataset) {}
