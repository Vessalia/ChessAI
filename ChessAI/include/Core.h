#pragma once
#include <SDL.h>
#include <iostream>

void Assert(bool cond);

#ifndef NDEBUG
#define DEBUG_ASSERT(cond) Assert(cond)
#define FATAL_ASSERT(cond) DEBUG_ASSERT(cond)
#else
#define DEBUG_ASSERT(cond)
#define FATAL_ASSERT(cond) \
    if (!(cond)) \
    { \
        std::cerr << "Fatal error reached: " << std::string(#cond) << std::endl; \
        SDL_Quit(); \
    }
#endif

constexpr size_t SCREEN_WIDTH = 720;
constexpr size_t SCREEN_HEIGHT = 640;
