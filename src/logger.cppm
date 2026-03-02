module;
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <format>
#include <optional>
#include <chrono>

import Config;

namespace fs = std::filesystem;
namespace ch = std::chrono;

export module Logger;

export class Logger {
    std::string name;

    static std::string& getPath();
    static std::string& getFileName();
    static std::ofstream& getFile();

    std::string getCurrentTime();
    std::string format(std::string log, std::optional<std::string> logLevel);
    void write(const std::string& log);

public:
    Logger(std::string Name);
    //~Logger();

    void Log(std::string log);
    void Log(std::string log, std::string logLevel);
};

std::string getFormattedCurrentTime() {
    auto now = ch::system_clock::now();
    auto time = ch::system_clock::to_time_t(now);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&time), "%Y-%m-%dT%H:%M:%S");

    return ss.str();
}

std::string& Logger::getPath() {
    static Config& config = Config::getInstance();
    static std::string path = config.Get<std::string>("LoggerPath", "./logs");

    if (!fs::exists(path)) {
        fs::create_directory(path);
    }

    return path;
}

std::string& Logger::getFileName() {
#ifdef NDEBUG
    std::string buildType = "Release";
#else
    std::string buildType = "Debug";
#endif

    static std::string fileName = std::format("{}_my3DGame_{}_last.log", getFormattedCurrentTime(), buildType);

    return fileName;
}

std::ofstream& Logger::getFile() {
    static std::ofstream file(std::format("{}/{}", getPath(), getFileName()), std::ios::app);

    return file;
}

std::string Logger::getCurrentTime() {
    return getFormattedCurrentTime();
}

std::string Logger::format(std::string log, std::optional<std::string> logLevel) {
    std::string LogLevel = logLevel.has_value() ? std::format(" {}", *logLevel) : "";

    return std::format("{};{} [{}]: {}\n", getCurrentTime(), LogLevel, name, log);
}

void Logger::write(const std::string& log) {
    getFile() << log;
    getFile().flush();
}

Logger::Logger(std::string Name) : name(Name) {};

void Logger::Log(std::string log) {
    std::string formattedStr = format(log, std::nullopt);
    write(formattedStr);

    std::cout << formattedStr;
}

void Logger::Log(std::string log, std::string logLevel) {
    std::string formattedStr = format(log, logLevel);
    write(formattedStr);

    std::cout << formattedStr;
}