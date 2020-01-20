#pragma once

#include "logger.h"

#define  SML_LOG_LINE(str, ...) SmartLib::Logger::LoggerInstance().Log(str, __VA_ARGS__)