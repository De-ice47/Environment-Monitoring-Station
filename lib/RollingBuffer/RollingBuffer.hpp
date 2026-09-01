#pragma once

#include <cstdint>
#include <cstddef>

template<typename T, std::size_t Capacity>
class RollingBuffer
{
private:

    T data[Capacity];

    std::size_t start = 0;
    std::size_t count = 0;

public:

    bool Push(const T& value)
    {
        if (Capacity == 0)
            return false;

        // Position where the new item goes
        std::size_t index = (start + count) % Capacity;

        data[index] = value;

        if(count < Capacity)
        {
            count++;
        }
        else
        {
            // Buffer was full, so the oldest
            // item has just been overwritten.
            start = (start + 1) % Capacity;
        }

        return true;
    }
    std::size_t Size() const
    {
        return count;
    }
    constexpr std::size_t CapacitySize() const
    {
        return Capacity;
    }
    bool Empty() const
    {
        return count == 0;
    }
    bool Full() const
    {
        return count == Capacity;
    }
    void Clear()
    {
        start = 0;
        count = 0;
    }
    T& operator[](std::size_t index)
    {
        std::size_t actualIndex = (start + index) % Capacity;
        return data[actualIndex];
    }
    const T& operator[](std::size_t index) const
    {
        std::size_t actualIndex = (start + index) % Capacity;
        return data[actualIndex];
    }
    T& Oldest()
    {
        return data[start];
    }
    const T& Oldest() const
    {
        return data[start];
    }
    T& Newest()
    {
        std::size_t index = (start + count - 1) % Capacity;
        return data[index];
    }
    const T& Newest() const
    {
        std::size_t index = (start + count - 1) % Capacity;
        return data[index];
    }
};