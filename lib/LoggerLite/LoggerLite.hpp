#pragma once
#include <cstdint>
#include <string>

namespace LL
{
    constexpr uint8_t MAX_LINES = 256;

    void Print(const std::string& message);
    void Println(const std::string& message);

    uint16_t GetLineCount();
    const std::string& GetLine(uint16_t index);
    const std::string GetLines(uint16_t index, uint8_t count);

    void Clear();
}