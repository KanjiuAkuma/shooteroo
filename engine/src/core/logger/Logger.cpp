/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine {

    std::shared_ptr<spdlog::logger> Logger::logger;

    void Logger::init(std::string applicationName)
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        logger = spdlog::stdout_color_mt(applicationName);
        logger->set_level(spdlog::level::trace);
    }

}