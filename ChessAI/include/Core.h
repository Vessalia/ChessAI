#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>

#define PREMAGICS

void Assert(bool cond);
unsigned int GetRandU32();
uint64_t GetRandU64();
uint64_t GetMagicNumber();
std::vector<std::string> Tokenize(const std::string& tokens, const std::string& delimiter);

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

extern size_t SCREEN_WIDTH;
extern size_t SCREEN_HEIGHT;
