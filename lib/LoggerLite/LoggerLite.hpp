#pragma once
#include <cstdint>
#include <cstring>

namespace LL
{
    constexpr uint8_t MAX_LINES = 255;
    constexpr uint8_t MAX_LINE_LENGTH = 64;

    using OutputCallback = void (*)(const char* message);

    void Initialize(OutputCallback output = nullptr);

    void Println(const char* message);

    uint8_t GetLineCount();

    const char* GetLine(uint8_t index);

    void Clear();
}