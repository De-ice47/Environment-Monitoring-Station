#pragma once
#include <cstdint>
#include <cstring>

namespace LL
{
    constexpr std::uint8_t MAX_LINES = 255;
    constexpr std::uint8_t MAX_LINE_LENGTH = 64;

    using OutputCallback = void (*)(const char* message);

    void Initialize(OutputCallback output = nullptr);

    void Println(const char* message);

    std::uint8_t GetLineCount();

    const char* GetLine(std::uint8_t index);

    void Clear();
}