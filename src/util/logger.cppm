module;
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <format>
#include <optional>
#include <chrono>
#include <utility>

export module Logger;
import osinfo;

namespace fs = std::filesystem;
namespace ch = std::chrono;

export class Logger {
    std::string_view name;

    static fs::path& getPath();
    static std::string& getFileName();
    static std::ofstream& getFile();

    std::string getCurrentTime();
    std::string format(std::string_view log, std::optional<std::string_view> logLevel);
    void write(std::string_view log);

public:
    Logger(std::string_view Name);
    //~Logger();

    void Log(std::string_view log);
    void Log(std::string_view log, std::string_view logLevel);
    
    void Debug(std::string_view log);

    template <typename... Args>
    void Logf(std::format_string<Args...> fmt, Args&&... args);

    template <typename... Args>
    void Debugf(std::format_string<Args...> fmt, Args&&... args);
};

std::string getFormattedCurrentTime() {
    auto now = ch::system_clock::now();
    auto time = ch::system_clock::to_time_t(now);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&time), "%Y-%m-%dT%H:%M:%S");

    return ss.str();
}

fs::path& Logger::getPath() {
    static fs::path path = os::appPath / "logs";

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
    static std::ofstream file(getPath() / getFileName(), std::ios::app);

    return file;
}

std::string Logger::getCurrentTime() {
    return getFormattedCurrentTime();
}

std::string Logger::format(std::string_view log, std::optional<std::string_view> logLevel) {
    std::string LogLevel = logLevel.has_value() ? std::format(" {}", *logLevel) : "";

    return std::format("{};{} [{}]: {}\n", getCurrentTime(), LogLevel, name, log);
}

void Logger::write(std::string_view log) {
    getFile() << log;
    getFile().flush();
}

Logger::Logger(std::string_view Name) : name(Name) {};

void Logger::Log(std::string_view log) {
    std::string formattedStr = format(log, std::nullopt);
    write(formattedStr);

    std::cout << formattedStr;
}

void Logger::Log(std::string_view log, std::string_view logLevel) {
    std::string formattedStr = format(log, logLevel);
    write(formattedStr);

    std::cout << formattedStr;
}

void Logger::Debug(std::string_view log) {
#ifdef NDEBUG
    return;
#endif

    Log(log, (std::string_view)"DEBUG");
}

template <typename... Args>
void Logger::Logf(std::format_string<Args...> fmt, Args&&... args) {
    std::string s = std::format(fmt, std::forward<Args>(args)...);

    Log(s);
}


template <typename... Args>
void Logger::Debugf(std::format_string<Args...> fmt, Args&&... args) {
    std::string s = std::format(fmt, std::forward<Args>(args)...);
    
    Debug(s);
}