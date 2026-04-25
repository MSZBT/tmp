#ifndef LOGGER_H
#define LOGGER_H


#include <spdlog/spdlog.h>

class Logger : public spdlog::logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
private:
    Logger();
};

#endif