#include "Logger.h"
#include <ctime>

std::vector<LogEntry> Logger::messages;

enum TEXT_COLOR {
    RED = 31,
    GREEN = 32,
    YELLOW = 33, 
    BRIGHT_BLACK = 91,
    BRIGHT_GREEN = 92
};

void SetConsoleColor(TEXT_COLOR textColor){
    std::cout << "\033[" << textColor << "m";
}

void ResetConsoleColor() { std::cout << "\033[0m"; }

std::string GetDateTimeInfo(char* dateBuffer){
    time_t rawTime;
    struct tm * timeinfo;

    time(&rawTime);
    timeinfo = localtime(&rawTime);

    strftime(dateBuffer, 80, "%d-%b-%G %T" ,timeinfo); 
    return dateBuffer;
}

void Logger::Log(const std::string& message){
    char dateHandler[80];
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "LOG | " + GetDateTimeInfo(dateHandler) + " - " + message;
    
    SetConsoleColor(BRIGHT_GREEN);
    std::cout << logEntry.message << "\033[0m" << std::endl;

    messages.push_back(logEntry);
}

void Logger::Err(const std::string& message){
    char dateHandler[80];
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERR | " + GetDateTimeInfo(dateHandler) + " - " + message;

    SetConsoleColor(RED);
    std::cout << logEntry.message << "\033[0m" << std::endl;

    messages.push_back(logEntry);
}