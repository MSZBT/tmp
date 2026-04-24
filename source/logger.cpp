#include "logger.h"

Logger::Logger() : spdlog::logger("logger") {

    this->set_level(spdlog::level::info);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    this->sinks().push_back(console_sink);

    info("Logger initialized");
}
