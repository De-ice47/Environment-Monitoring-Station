#include "LoggerLite.hpp"
#include <Arduino.h>
namespace LL
{
    char lines[MAX_LINES][MAX_LINE_LENGTH];

    std::uint8_t lineCount = 0;
    std::uint8_t oldestLine = 0;

    OutputCallback output = nullptr;


    void Initialize(OutputCallback outputCallback)
    {
        output = outputCallback;
        Clear();
    }


    void Println(const char* message)
    {
        if(message == nullptr)
            return;

        // Forward message to external output.
        if(output != nullptr)
        {
            output(message);
        }


        // Determine where to store the message.
        std::uint8_t index;

        if(lineCount < MAX_LINES)
        {
            index = (oldestLine + lineCount) % MAX_LINES;

            lineCount++;
        }
        else
        {
            index = oldestLine;

            oldestLine =
                (oldestLine + 1) % MAX_LINES;
        }


        // Copy message into buffer.
        std::strncpy(
            lines[index],
            message,
            MAX_LINE_LENGTH - 1
        );

        // Always guarantee null termination.
        lines[index][MAX_LINE_LENGTH - 1] = '\0';
    }


    std::uint8_t GetLineCount()
    {
        return lineCount;
    }


    const char* GetLine(std::uint8_t index)
    {
        if(index >= lineCount)
            return "";

        std::uint8_t actualIndex =
            (oldestLine + index) % MAX_LINES;

        return lines[actualIndex];
    }

    void Clear()
    {
        for(std::uint8_t i = 0; i < MAX_LINES; i++)
        {
            lines[i][0] = '\0';
        }

        lineCount = 0;
        oldestLine = 0;
    }
}