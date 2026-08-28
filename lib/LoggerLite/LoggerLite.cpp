#include "LoggerLite.hpp"

namespace LoggerLite
{
    std::string lines[MAX_LINES];

    uint8_t lineCount = 0;
    uint8_t oldestLine = 0;

    void Println(const std::string& message)
    {
        if(lineCount < MAX_LINES)
        {
            uint8_t index =
                (oldestLine + lineCount) % MAX_LINES;

            lines[index] = message;
            lineCount++;
        }
        else
        {
            lines[oldestLine] = message;

            oldestLine =
                (oldestLine + 1) % MAX_LINES;
        }
    }

    uint8_t GetLineCount()
    {
        return lineCount;
    }

    const std::string& GetLine(uint8_t index)
    {
        if(index >= lineCount)
        {
            static const std::string empty = "";
            return empty;
        }

        uint8_t actualIndex =
            (oldestLine + index) % MAX_LINES;

        return lines[actualIndex];
    }

    std::string GetLines(uint8_t index, uint8_t count)
    {
        if(index >= lineCount)
            return "";

        if(index + count > lineCount)
            return "";

        std::string result;

        for(uint8_t i = 0; i < count; i++)
        {
            result += GetLine(index + i);
            result += '\n';
        }

        return result;
    }

    void Clear()
    {
        for(uint8_t i = 0; i < MAX_LINES; i++)
            lines[i].clear();

        lineCount = 0;
        oldestLine = 0;
    }
}