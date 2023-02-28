#pragma once


#include "Serializable.h"


struct AbstactBitField : public Serializable
{
    virtual int bitlength() const = 0;
};


struct FieldWithValue : public AbstactBitField
{
    virtual void setValue(double) = 0;
    virtual double value() const = 0;
};
