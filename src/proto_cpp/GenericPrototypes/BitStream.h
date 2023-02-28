#pragma once


#if __cplusplus >= 201103L
#include <cstdint>
#else
#include <stdint.h>
#endif
#include <vector>
#include <limits>

#include "BitField.h"
#include "BitFieldAligner.h"


//------------------------------------------------------------------------------

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wsign-conversion"


namespace FieldPack
{


class PackStrategy
{
public:
    explicit PackStrategy(std::vector<uint8_t>& memory)
        : data(memory)
    {}
    
    void push(uint32_t value, uint8_t length)
    {
        if (offset == 0)
            data.push_back(0);

        auto last = data.rbegin();
        *last |= value << offset;

        offset += length;
        if (offset == sizeof(uint8_t) * 8)
            offset = 0;
    }
    
    uint8_t pop(uint8_t length)
    {
        auto value = data.at(read_pos);

        uint8_t mask = (length == sizeof(uint8_t) * 8) 
            ? std::numeric_limits<uint8_t>::max() 
            : static_cast<uint8_t>((1 << length) - 1);
        value = (value >> des_offset) & mask;

        des_offset += length;
        if (des_offset == sizeof(uint8_t) * 8)
        {
            des_offset = 0;
            read_pos++;
        }

        return value;
    }
    
    
protected:
    uint8_t offset = 0;
    uint8_t des_offset = 0;
    uint32_t read_pos = 0;
    
public:
    std::vector<uint8_t>& data;
};

/**
 * Стратегия плотной упаковки битовых полей.
 */
class Tight : public PackStrategy
{
public:
    Tight(std::vector<uint8_t>& memory)
        : PackStrategy(memory)
    {}

    template<typename T>
    void serialize(T value, uint8_t length)
    {
        uint8_t freebits = sizeof(uint8_t) * 8 - offset;
        while (freebits < length)
        {
            uint32_t freebits_mask = (freebits == sizeof(uint8_t) * 8)
                ? std::numeric_limits<uint8_t>::max()
                : static_cast<uint32_t>((1 << freebits) - 1);
            push(static_cast<uint32_t>(value) & freebits_mask, freebits);
            value >>= freebits;

            length -= freebits;
            freebits = sizeof(uint8_t) * 8 - offset;
        }
        push(static_cast<uint32_t>(value) & std::numeric_limits<uint32_t>::max(), length);
    }
    
    template<typename T>
    void deserialize(T& value, uint8_t length)
    {
        uint8_t preshiftValue = 0;
        uint8_t readbits = sizeof(uint8_t) * 8 - des_offset;
        while (readbits < length)
        {
            value |= (T(pop(readbits)) << preshiftValue);

            length -= readbits;
            preshiftValue += readbits;
            readbits = sizeof(uint8_t) * 8 - des_offset;
        }
        value |= (T(pop(length)) << preshiftValue);
    }

    void serialize(const float &value, uint8_t length)
    {
        serialize(reinterpret_cast<const uint32_t&>(value),length);
    }

    void serialize(const double &value, uint8_t length)
    {
        serialize(reinterpret_cast<const uint64_t&>(value),length);
    }

    void deserialize(float& value, uint8_t length)
    {
        deserialize(reinterpret_cast<uint32_t&>(value),length);
    }

    void deserialize(double& value, uint8_t length)
    {
        deserialize(reinterpret_cast<uint64_t&>(value),length);
    }
};


class WithoutFieldBreaking : public PackStrategy
{
public:
    WithoutFieldBreaking(std::vector<uint8_t>& memory)
        : PackStrategy(memory)
    {}

    template <typename T>
    void serialize(T value, uint8_t length)
    {
        uint8_t freebits = sizeof(uint8_t) * 8 - offset;
        if (freebits < length)
            offset = 0;
        push(value & ((uint64_t(1) << length) - 1), length);
    }
    
    template<typename T>
    void deserialize(T& value, uint8_t length)
    {
        uint8_t readbits = sizeof(uint8_t) * 8 - des_offset;
        if (readbits < length)
            pop(readbits);
        value |= pop(length);
    }
};


} // namespace FieldPack


//------------------------------------------------------------------------------


class BitStream final : public FieldPack::Tight
{
public:

    BitStream(std::vector<uint8_t>& memmory)
        : Tight(memmory)
    {
    }

    // Serializers
    
    template <typename T>
    BitStream& operator<<(const typename std::enable_if<std::is_fundamental<T>::value, T>::type& value)
    {
        serialize(value, sizeof (value) * 8);
        
        return *this;
    }

    template <typename ValueType, uint8_t FieldLenght>
    BitStream& operator<<(const BitField<ValueType, FieldLenght>& field)
    {
        serialize(static_cast<ValueType>(field), field.length());
        
        return *this;
    }

    template <uint8_t AlignLength>
    BitStream& operator<<(const BitFieldAligner<AlignLength>& aligner)
    {
        auto bitlen = (data.size() * sizeof(uint8_t) * 8 + offset);
        if (offset)
            bitlen -= sizeof(uint8_t) * 8;
        auto len = bitlen % aligner.length();
        if (len == 0)
            return *this;

        serialize(0, aligner.length() - len);
        
        return *this;
    }

    // Deserializes

    template <typename T>
    BitStream& operator>>(typename std::enable_if<std::is_fundamental<T>::value, T>::type& value)
    {
        deserialize(value = 0, sizeof (value) * 8);
        
        return *this;
    }

    template <typename ValueType, uint8_t FieldLenght>
    BitStream& operator>>(BitField<ValueType, FieldLenght>& field)
    {
        ValueType v = 0;
        deserialize(v, field.length());
        field = v;

        return *this;
    }

    template <uint8_t AlignLength>
    BitStream& operator>>(BitFieldAligner<AlignLength>& aligner)
    {
        uint8_t len = (read_pos * sizeof(uint8_t) * 8 + des_offset) % aligner.length();
        if (len == 0)
            return *this;

        len = aligner.length() - len;

        uint32_t temp;
        deserialize(temp, len);
        
        return *this;
    }
};

#pragma GCC diagnostic pop
