#pragma once


#include "AbstractBitField.h"


template <uint8_t AlignLength>
class BitFieldAligner : public AbstactBitField
{
public:
    constexpr uint8_t length() const
    {
        return AlignLength;
    }
    
    int bitlength() const override
    {
        return -(1 + AlignLength);
    }

    BitStream& serialize(BitStream& stream) const override
    {
        return stream << *this;
    }

    BitStream& deserialize(BitStream& stream) override
    {
        return stream >> *this;
    }
};
