#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <Arduino.h>
#include "config.h"

class CommandParser {
private:
    bool measuring;
    
public:
    CommandParser();
    void handleCommand(const String& command); // осторожно, ссылка!
    void printHelp();
    bool isMeasuring() { return measuring; }
};

extern CommandParser commandParser;

#endif