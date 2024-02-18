#pragma once

#include <iostream>

#define LOG_ASSERT(condition, message) \
    if (!(condition)) { \
        std::cerr << message << std::endl; \
        ::abort(); \
    }