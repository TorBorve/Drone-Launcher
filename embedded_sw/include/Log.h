#pragma once

#include <TeensyThreads.h>

#include "Communicator.h"

#define LOG_DEBUG(msg) { \
    Threads::Scope lock(communicator.mutex); \
    communicator.nh.logdebug(msg);}

#define LOG_INFO(msg) { \
    Threads::Scope lock(communicator.mutex); \
    communicator.nh.loginfo(msg);}

#define LOG_WARN(msg) { \
    Threads::Scope lock(communicator.mutex); \
    communicator.nh.logwarn(msg);}

#define LOG_ERROR(msg) { \
    Threads::Scope lock(communicator.mutex); \
    communicator.nh.logerror(msg);}