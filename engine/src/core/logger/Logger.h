#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Engine {

    class Logger {
    public:
        static void init(std::string applicationName);

        inline static std::shared_ptr<spdlog::logger>& getLogger() { return logger; }

    private:
        static std::shared_ptr<spdlog::logger> logger;
    };

}

#define LOG_TRACE(...)         ::Engine::Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)          ::Engine::Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          ::Engine::Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         ::Engine::Logger::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      ::Engine::Logger::getLogger()->critical(__VA_ARGS__)

#ifdef NDEBUG
    #define LOG_DEBUG(...)
#else
    #define LOG_DEBUG(...)      ::Engine::Logger::getLogger()->debug(__VA_ARGS__)
#endif
