#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <optional>

using std::string;

class Logger {
    string name;

    static string& getPath();
    static string& getFileName();
    static std::ofstream& getFile();

    string getCurrentTime();
    string format(string log, std::optional<string> logLevel);
    void write(const string& log);

public:
    Logger(string Name);
    //~Logger();

    void Log(string log);
};

#endif