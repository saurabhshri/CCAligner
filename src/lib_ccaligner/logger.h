/*
* Author   : Harry Yu
* Email    : harryyunull@gmail.com
* Link     : https://github.com/harrynull
*/

#ifndef CCALIGNER_LOGGER_H
#define CCALIGNER_LOGGER_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <array>
#include <exception>
#include <typeinfo>

// Define possible exit codes that will be passed on to the fatal function exit codes.
struct Dummy {};
struct FileNotFound : public std::runtime_error {
    FileNotFound(std::string reason) noexcept : std::runtime_error(std::move(reason)) {}
};
struct InvalidParameters : public std::runtime_error {
    InvalidParameters(std::string reason) noexcept : std::runtime_error(std::move(reason)) {}
};
struct IncompatibleParameters : public std::runtime_error {
    IncompatibleParameters(std::string reason) noexcept : std::runtime_error(std::move(reason)) {}
};
struct IncompleteParameters : public std::runtime_error {
    IncompleteParameters(std::string reason) noexcept : std::runtime_error(std::move(reason)) {}
};
struct InvalidFile : public std::runtime_error {
    InvalidFile(std::string reason) noexcept : std::runtime_error(std::move(reason)) {}
};
struct UnknownError : public std::runtime_error {
    UnknownError(std::string reason) noexcept : std::runtime_error(std::move(reason)) {}
};

namespace Colors {
    using ColorFunc = std::ostream&(std::ostream&);
#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#undef ERROR
    inline void setConsoleColor(short colorCode) {
        static HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout, colorCode);
    }

#define WINDOWS_COLOR(colorName, colorCode)\
    inline std::ostream& colorName(std::ostream& s){setConsoleColor(colorCode);return s;}

    WINDOWS_COLOR(red,      FOREGROUND_RED);
    WINDOWS_COLOR(yellow,   FOREGROUND_RED | FOREGROUND_GREEN);
    WINDOWS_COLOR(lred,     FOREGROUND_RED | FOREGROUND_INTENSITY);
    WINDOWS_COLOR(white,    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    WINDOWS_COLOR(lyellow,  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    WINDOWS_COLOR(lwhite,   FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    WINDOWS_COLOR(cdefault, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

#undef WINDOWS_COLOR

#else
    inline void setDefaultColor() {}
#define LINUX_COLOR(colorName, colorCode) inline std::ostream& colorName(std::ostream &s) {return s << colorCode;}

    LINUX_COLOR(red,        "\033[21;31m");
    LINUX_COLOR(yellow,     "\033[21;33m");
    LINUX_COLOR(lred,       "\033[1;31m");
    LINUX_COLOR(white,      "\033[21;37m");
    LINUX_COLOR(lyellow,    "\033[1;33m");
    LINUX_COLOR(lwhite,     "\033[1;37m");
    LINUX_COLOR(cdefault,   "\033[0;00m");
    
#undef LINUX_COLOR
#endif
}

class Logger {
public:

    enum Level {
        verbose, debug, info, warning, error, fatal, nolog
    };

    static std::array<const char*, 6> levelTags;
    static std::array<Colors::ColorFunc*, 6> colors;

    // A sink -- where the log will go to. It can be stdout, a file, or any custom ostream (like network).
    class Sink {
    public:
        Sink(std::ostream& os, bool useColor) :_os(os), _useColor(useColor) {}

        // Write stringstream to ostream if the level is higher than the minimum output level.
        void output(const std::stringstream& ss, Level level) {
            if (level < _minimumOutputLevel) return;
            if (_useColor) _os << *colors[level];
            _os << ss.str();
            if (_useColor) _os << Colors::cdefault; // change back to the deafult color
            if (level == error || level == fatal || true) flush(); // flush immediately to prevent data loss.
        }

        // Set thte minimum output level.
        void setMinimumOutputLevel(Level level) noexcept {
            _minimumOutputLevel = level;
        }

        void flush() {
            _os.flush();
        }

    private:
        std::ostream& _os;
        Level _minimumOutputLevel = Level::debug;
        bool _useColor;
    };

    // Exception Type is used to indicate error when there is an exception. It is void for normal logs.
    template <class ExceptionType = Dummy>
    class Log {
    public:
        Log(Logger& logger, const char* fileName, const char* funcName, int lineNumber, Level level)
            :_logger(logger), _level(level) {
            _ss << "[" << getCurrentTime() << "]" << levelTags[level] << " ";
            if (level == error || level == fatal) _ss << fileName << " (" << lineNumber << ") : " << funcName << " | ";
        }

        template <typename T>
        Log& operator<<(const T& rhs) {
            _ss << rhs;
            return *this;
        }

        ~Log() noexcept(false) {
            _ss << std::endl;
            _logger.log(_ss, _level);
            if (!std::uncaught_exception()) { // to avoid two uncaught exceptions.
                throwExceptionIfNeeded();
            }
            else {
                std::cerr << "Another exception occurred during unwinding." << std::endl;
                std::cerr << "Exception details:" << std::endl
                    << "Type: " << typeid(ExceptionType).name() << ". " << std::endl
                    << "Reason: " << _ss.str() << std::endl;
            }
        }

    private:
        template<class E = ExceptionType>
        typename std::enable_if<std::is_same<E, Dummy>::value>::type throwExceptionIfNeeded() {}
        template<class E = ExceptionType>
        typename std::enable_if<!std::is_same<E, Dummy>::value>::type throwExceptionIfNeeded() noexcept(false) {
            throw E(_ss.str());
        }

        static std::string getCurrentTime() {
            char currentTime[16];
            const auto now = std::time(nullptr);
            std::strftime(currentTime, 16, "%m-%d %H:%M:%S", std::localtime(&now));
            return std::string(currentTime);
        }

        Logger& _logger;
        std::stringstream _ss;
        Level _level;
    };

    // It will apply the level to all **existing** sinks. Set level to nolog if you don't want any log.
    // default: debug
    void setMinimumOutputLevel(Level level) noexcept {
        for (auto& sink : _sinks) sink.setMinimumOutputLevel(level);
    }

    void log(std::stringstream& ss, Level level) {
        for (auto& sink : _sinks) sink.output(ss, level);
    }

    void addSink(Sink sink) {
        _sinks.emplace_back(sink);
    }

    void flush() {
        for (auto& sink : _sinks) sink.flush();
    }

    friend Logger& getLogger();

private:
    Logger() : _sinks{ {std::cout, true} } { }
    std::vector<Sink> _sinks;
};

inline Logger& getLogger() {
    static Logger logger;
    return logger;
}

#define loggerstream(level) (Logger::Log<>(getLogger(), __FILE__, __FUNCTION__, __LINE__, Logger::Level::level))
// Information for tracing
#define VERBOSE loggerstream(verbose)
// Information for developers
#define DEBUG loggerstream(debug)
// Information for general users
#define INFO loggerstream(info)
// Problems that may affect facility, performance or stability but may not lead the program to crash immediately
#define WARNING loggerstream(warning)
// Something unexpected happened but can be recovered
#define ERROR loggerstream(error)
// Unrecoverable error and program termination is required
#define FATAL(exception) (Logger::Log<exception>(getLogger(), __FILE__, __FUNCTION__, __LINE__, Logger::Level::fatal))

#endif
