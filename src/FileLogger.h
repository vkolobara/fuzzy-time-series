//
// Created by vkolobara on 6/2/18.
//

#ifndef FUZZY_TIME_SERIES_FILELOGGER_H
#define FUZZY_TIME_SERIES_FILELOGGER_H


#include <ECF/ECF.h>

class FileLogger {
private:
    ofstream file;
public:
    explicit FileLogger(string filePath);
    ~FileLogger();
    void writeHeader();
    void log(uint genNo, double trainValue, double testValue);
};


#endif //FUZZY_TIME_SERIES_FILELOGGER_H
