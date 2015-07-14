
#pragma once

#include <cstdint>

template<typename T, typename SizeType = std::int32_t>
class Array
{
public:
    Array():
        m_array(nullptr),
        m_length(0)
    {
    }

    Array(const Array& other) = delete;
    Array& operator=(const Array& other) = delete;

    Array(Array&& other):
        m_array(other.m_array),
        m_length(other.m_length)
    {
        other.m_array = nullptr;
        other.m_length = 0;
    }

    ~Array()
    {
        delete[] m_array;
    }

    Array& operator=(Array&& other)
    {
        m_array = other.m_array;
        m_length = other.m_length;
        other.m_array = nullptr;
        other.m_length = 0;
        return *this;
    }

    bool reset(SizeType length)
    {
        delete[] m_array;
        if (length > 0)
        {
            m_array = new T[length];
            m_length = length;
        }
        else // if (length <= 0)
        {
            m_array = nullptr;
            m_length = 0;
        }
        return (length >= 0);
    }

    bool resize(SizeType length)
    {
        return reset(length);
    }

    T* begin() const
    {
        return m_array;
    }

    T* end() const
    {
        return m_array + m_length;
    }

    SizeType length() const
    {
        return m_length;
    }

    T& operator[](SizeType index) const
    {
        return *(m_array + index);
    }

private:
    T* m_array;
    SizeType m_length;
};
