#pragma once


#include <cstdlib>
#include <type_traits>
#include <cmath>
#include <cinttypes>

#include "AbstractBitField.h"
#include "lsb.h"


template <typename ValueType, uint8_t FieldLenght, typename Enable = void>
class BitField;

template <typename ValueType, uint8_t FieldLenght>
class BitField<ValueType, FieldLenght, typename std::enable_if< std::is_integral<ValueType>::value, void>::type >
    : public FieldWithValue
{
    enum
    {
        FieldMask = (uint64_t(1) << FieldLenght) - 1,
        NegativeValueMask = ~FieldMask
    };


public:
    BitField()
        : BitField(ValueType(0), lsb(1.0))
    {}

    BitField(ValueType val, const lsb& weight)
        : m_data({val})
        , m_lsb(weight)
    {}

    BitField(const lsb& weight)
        : BitField(0, weight)
    {}

    BitField(ValueType val)
        : BitField(val, lsb())
    {}

    operator ValueType () const
    {
        static_assert(sizeof(ValueType) * 8 >= FieldLenght, "the wrong type of data to the length of the bit field");
        static_assert(std::is_integral<ValueType>::value, "the data type must be integral type");
        return m_data.value;
    }
    
    BitField& operator = (const ValueType& aValue)
    {
        m_data.value = aValue;
        return *this;
    }
    
    uint8_t length() const
    {
        return FieldLenght;
    }

    double lsbWeight() const
    {
        return m_lsb.weight;
    }

    double value() const override
    {
        return (m_lsb.weight * m_data.value);
    }

    void setValue(double value) override
    {
        m_data.value = static_cast<ValueType>(::round(value / m_lsb.weight));
    }
    
    int bitlength() const override
    {
        return FieldLenght;
    }

    BitStream& serialize(BitStream& stream) const override
    {
        return stream << *this;
    }
    
    BitStream& deserialize(BitStream& stream) override
    {
        return stream >> *this;
    }


private:
    struct Data
    {
        ValueType value : FieldLenght;    
    } m_data;
    lsb m_lsb;
};
