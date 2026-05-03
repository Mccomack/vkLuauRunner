export module logger;
import osinfo;
import appinfo;

import std;

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

std::stringstream getFormattedDate() {
    auto now = ch::system_clock::now();
    auto time = ch::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time), "%Y-%m-%d");

    return ss;
}

std::stringstream getFormattedTime() {
    auto now = ch::system_clock::now();
    auto time = ch::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time), "%H:%M:%S");

    return ss;
}

std::stringstream getFormattedTimeMicrosecond() {
    auto now = ch::system_clock::now();
    auto us = ch::duration_cast<ch::microseconds>(now.time_since_epoch()) % 1000000;

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(6) << us.count();

    return ss;
}

fs::path& Logger::getPath() {
    static fs::path path = os::appPath / "logs";

    if (!fs::exists(path)) {
        fs::create_directory(path);
    }

    return path;
}

std::string& Logger::getFileName() {
    std::string buildType = app::isDebugBuild ? "Debug" : "Release";

    static std::string fileName = std::format("{}T{}Z_{}_{}_last.log", getFormattedDate().str(), getFormattedTime().str(), app::name, buildType);

    return fileName;
}

std::ofstream& Logger::getFile() {
    static std::ofstream file(getPath() / getFileName(), std::ios::app);

    return file;
}

std::string Logger::getCurrentTime() {
    return std::format("{}T{}.{}Z", getFormattedDate().str(), getFormattedTimeMicrosecond().str(), getFormattedTimeMicrosecond().str());
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