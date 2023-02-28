#pragma once


#include "BitStream.h"
#include "AbstractBitField.h"
#include "BitFieldAligner.h"
#include "Serializable.h"


class Codogramm : public Serializable
{    
public:
    Codogramm() = default;
    ~Codogramm() override = default;
    
    Codogramm(const Codogramm&) {}
    Codogramm& operator = (const Codogramm&) { return *this; }
    
    int bitlength() const
    {
        int len = 0;
        for (auto it : order)
        {
            int bitlen = it->bitlength();
            if (bitlen < 0)
            {
                int align = -(bitlen + 1);

                int mod = len % align;
                if (mod != 0)
                    len += align - mod;
            }
            else
                len += bitlen;
        }
        return len;
    }
    
    int length() const
    {
        // Translate from bits representation to bytes. 3 = divide by 8.
        return (bitlength() >> 3);
    }

    BitStream& serialize(BitStream& stream) const override
    {
        for (auto & it : order)
            it->serialize(stream);
        return stream;
    }

    BitStream& deserialize(BitStream& stream) override
    {
        for (auto & it : order)
            it->deserialize(stream);
        return stream;
    }

    
protected:
    void appendToOrder(std::initializer_list<AbstactBitField*> fieldOrder)
    {
        order.insert( order.end(), fieldOrder );
    }


protected:
    BitFieldAligner<8> alignOn_8bit;
    BitFieldAligner<16> alignOn_16bit;
    BitFieldAligner<32> alignOn_32bit;
    

private:
    std::vector< AbstactBitField* > order;
};


inline BitStream& operator << (BitStream& stream, const Codogramm& cdg)
{
    return cdg.serialize(stream);
}

inline BitStream& operator >> (BitStream& stream, Codogramm& cdg)
{
    return cdg.deserialize(stream);
}
