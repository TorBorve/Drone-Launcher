#pragma once

#include <TeensyThreads.h>

#include "Communicator.h"

extern char _logBuffer[512];

#define LOG_DEBUG(...) { \
    Threads::Scope lock(communicator.mutex); \
    snprintf(_logBuffer, sizeof(_logBuffer)/sizeof(char), __VA_ARGS__); \
    communicator.nh.logdebug(_logBuffer);}

#define LOG_INFO(...) { \
    Threads::Scope lock(communicator.mutex); \
    snprintf(_logBuffer, sizeof(_logBuffer)/sizeof(char), __VA_ARGS__); \
    communicator.nh.loginfo(_logBuffer);}

#define LOG_WARN(...) { \
    Threads::Scope lock(communicator.mutex); \
    snprintf(_logBuffer, sizeof(_logBuffer)/sizeof(char), __VA_ARGS__); \
    communicator.nh.logwarn(_logBuffer);}

#define LOG_ERROR(...) { \
    Threads::Scope lock(communicator.mutex); \
    snprintf(_logBuffer, sizeof(_logBuffer)/sizeof(char), __VA_ARGS__); \
    communicator.nh.logerror(_logBuffer);}