//
// Created by vkolobara on 6/2/18.
//

#include "FileLogger.h"

inline bool fileExists (const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

FileLogger::FileLogger(string filePath) {
    bool exists = fileExists(filePath);
    file.open(filePath, ofstream::out | ofstream::app);

    if (!exists) {
        writeHeader();
    }
}

FileLogger::~FileLogger() {
    file.close();
}

void FileLogger::writeHeader() {
    file << "Generation,Train,Test" << endl;
}

void FileLogger::log(uint genNo, double trainValue, double testValue) {

    file << genNo << "," << trainValue << "," << testValue << endl;

}

