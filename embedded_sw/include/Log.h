#pragma once

#include <TeensyThreads.h>

#include "Communicator.h"

namespace DroneLauncher {

extern char _logBuffer[512];

}  // namespace DroneLauncher

#define LOG_DEBUG(...)                                                                                      \
    {                                                                                                       \
        Threads::Scope lock(DroneLauncher::communicator.mutex);                                             \
        snprintf(DroneLauncher::_logBuffer, sizeof(DroneLauncher::_logBuffer) / sizeof(char), __VA_ARGS__); \
        DroneLauncher::communicator.nh.logdebug(DroneLauncher::_logBuffer);                                 \
    }

#define LOG_INFO(...)                                                                                       \
    {                                                                                                       \
        Threads::Scope lock(DroneLauncher::communicator.mutex);                                             \
        snprintf(DroneLauncher::_logBuffer, sizeof(DroneLauncher::_logBuffer) / sizeof(char), __VA_ARGS__); \
        DroneLauncher::communicator.nh.loginfo(DroneLauncher::_logBuffer);                                  \
    }

#define LOG_WARN(...)                                                                                       \
    {                                                                                                       \
        Threads::Scope lock(DroneLauncher::communicator.mutex);                                             \
        snprintf(DroneLauncher::_logBuffer, sizeof(DroneLauncher::_logBuffer) / sizeof(char), __VA_ARGS__); \
        DroneLauncher::communicator.nh.logwarn(DroneLauncher::_logBuffer);                                  \
    }

#define LOG_ERROR(...)                                                                                      \
    {                                                                                                       \
        Threads::Scope lock(DroneLauncher::communicator.mutex);                                             \
        snprintf(DroneLauncher::_logBuffer, sizeof(DroneLauncher::_logBuffer) / sizeof(char), __VA_ARGS__); \
        DroneLauncher::communicator.nh.logerror(DroneLauncher::_logBuffer);                                 \
    }
