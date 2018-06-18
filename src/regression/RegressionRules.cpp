//
// Created by vkolobara on 6/18/18.
//

#include <FIS/parser/Helper.h>
#include "RegressionRules.h"
#include "../dataset/DataRow.h"

RegressionRules *RegressionRules::parseFile(string filePath) {
    ifstream file(filePath);

    vector<uint> antecedents;
    vector<vector<uint>> consequents;

    string line;

    // Skip headers
    while (getline(file, line)) {
        if (line == "") continue;
        vector<string> lineSplit = splitString(line, "->");

        auto consequentString = splitString(lineSplit[1], ",");

        uint antecedent = stoi(lineSplit[0]);
        vector<uint> consequent;
        for (auto a : consequentString) {
            consequent.push_back(stoi(a));
        }

        antecedents.push_back(antecedent);
        consequents.push_back(consequent);
    }

    file.close();

    return new RegressionRules(antecedents, consequents);
}

RegressionRules::RegressionRules(const vector<uint> &antecedents, const vector<vector<uint>> &consequents)
        : antecedents(antecedents), consequents(consequents) {}

uint RegressionRules::getTotalParams() {
    uint count = 0;

    for (auto c : consequents) {
        count += c.size();
    }

    return count;
}

