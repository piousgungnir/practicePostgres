#pragma once


#include <cstdlib>
#include <string>
#include <type_traits>
#include <cmath>

#include "AbstractBitField.h"
#include "lsb.h"
#include "BitStream.h"
#include "Codogram.h"


template <typename ValueType, typename Enable = void>
class Field;


template <typename ValueType>
class Field<ValueType, typename std::enable_if< std::is_base_of<Codogramm, ValueType>::value, void >::type> 
        : public virtual AbstactBitField
        , public ValueType
{	
public:
    int bitlength() const override
    {
        return ValueType::bitlength();
    }

    BitStream& serialize(BitStream& stream) const override
    {
        return ValueType::serialize(stream);
    }
    
    BitStream& deserialize(BitStream& stream) override
    {
        return ValueType::deserialize(stream);
    }
};


template<typename ValueType>
class Field<ValueType, typename std::enable_if< std::is_fundamental<ValueType>::value, void>::type >
    : public FieldWithValue
{
public:
    Field()
        : m_lsb(1.0)
    {}

    Field(const lsb& weight)
        : m_lsb(weight)
    {}
    
    operator ValueType () const
    {
        return fieldValue;
    }
    
    Field& operator = (const ValueType& aValue)
    {
        fieldValue = aValue;

        return *this;
    }
    
    constexpr uint8_t length() const
    {
        return sizeof(ValueType) * 8;
    }

    double lsbWeight() const
    {
        return m_lsb.weight;
    }

    double value() const override
    {
        return (m_lsb.weight * fieldValue);
    }

    void setValue(double value) override
    {
        fieldValue = ::round(value / m_lsb.weight);
    }
    
    int bitlength() const override
    {
        return static_cast<int>(sizeof(ValueType) * 8);
    }
    
    BitStream& serialize(BitStream& stream) const override
    {
        return stream.operator << <ValueType>(fieldValue);
    }
    
    BitStream& deserialize(BitStream& stream) override
    {
        return stream.operator >> <ValueType>(fieldValue);
    }

    
public:
    ValueType fieldValue = 0;
    lsb m_lsb;
};


/**
 * std::string specialization
 */
template<>
class Field<std::string>
    : public AbstactBitField
{
public:
    const std::string& value() const
    {
        return str;
    }

    void setValue(const std::string& str)
    {
        this->str.assign(str);
    }

    int bitlength() const override
    {
        return static_cast<int>(str.length() * 8);
    }

    BitStream& serialize(BitStream& stream) const override
    {
        uint32_t length = str.length();
        stream.serialize(length, sizeof(length) * 8);
        for (auto ch : str)
            stream.serialize(ch, sizeof(ch) * 8);
        return stream;
    }

    BitStream& deserialize(BitStream& stream) override
    {
        uint32_t length = 0;
        stream.deserialize(length, sizeof(length) * 8);
        str.resize(length);
        for (auto & ch : str)
            stream.deserialize(ch, sizeof(ch) * 8);
        return stream;
    }


private:
    std::string str = "";
};
