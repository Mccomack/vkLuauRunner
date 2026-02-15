#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <format>
#include <optional>
#include <chrono>
#include "logger.hpp"
#include "./../config/config.hpp"

namespace fs = std::filesystem;
namespace ch = std::chrono;

using std::string;

string getFormattedCurrentTime() {
    auto now = ch::system_clock::now();
    auto time = ch::system_clock::to_time_t(now);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&time), "%Y-%m-%dT%H:%M:%S");

    return ss.str();
}

string& Logger::getPath() {
    static Config& config = Config::getInstance();
    static string path = config.Get<string>("LoggerPath", "./logs");

    if (!fs::exists(path)) {
        fs::create_directory(path);
    }

    return path;
}

string& Logger::getFileName() {
    static string fileName = std::format("{}_my3DGame_last.log", getFormattedCurrentTime());

    return fileName;
}

std::ofstream& Logger::getFile() {
    static std::ofstream file(std::format("{}/{}", getPath(), getFileName()), std::ios::app);

    return file;
}

string Logger::getCurrentTime() {
    return getFormattedCurrentTime();
}

string Logger::format(string log, std::optional<string> logLevel) {
    string LogLevel = logLevel.has_value() ? std::format(" {}", *logLevel) : "";

    return std::format("{};{} [{}]: {}\n", getCurrentTime(), LogLevel, name, log);
}

void Logger::write(const string& log) {
    getFile() << log;
    getFile().flush();
}

Logger::Logger(string Name) : name(Name) {};

void Logger::Log(string log) {
    string formattedStr = format(log, std::nullopt);
    write(formattedStr);

    std::cout << formattedStr;
}

void Logger::Log(string log, string logLevel) {
    string formattedStr = format(log, logLevel);
    write(formattedStr);

    std::cout << formattedStr;
}