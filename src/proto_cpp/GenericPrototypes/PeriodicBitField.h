#pragma once


#include <vector>

#include "AbstractBitField.h"
#include "Field.h"
#include "BitStream.h"
#include "Codogram.h"


template <typename T>
class PeriodicBitFieldBase : public AbstactBitField, public std::vector<T>
{
public:
    template<typename ValueType>
    PeriodicBitFieldBase(Field<ValueType>& numberOf_field, int offset)
        : numberOf(numberOf_field)
        , offset(offset)
    {
    }
    
    template<typename ValueType, uint8_t FieldLenght>
    PeriodicBitFieldBase(BitField<ValueType, FieldLenght>& numberOf_field, int offset)
        : numberOf(numberOf_field)
        , offset(offset)
    {
    }

    PeriodicBitFieldBase(int numberOf, int offset)
        : numberOf(int_numberOf)
        , offset(offset)
    {
        int_numberOf = numberOf;
    }

    PeriodicBitFieldBase& operator = (const PeriodicBitFieldBase& src)
    {
        std::vector<T>::assign(src.begin(), src.end());
        return *this;
    }

    T& operator [](size_t index)
    {
        if (this->size() <= index)
            this->resize(index + 1);

        return *(this->data() + index);
    }

protected:
    FieldWithValue& numberOf;
    Field<int> int_numberOf;
    int offset = 0;
};



template <typename T, typename Enable = void>
class PeriodicBitField;


template <typename T>
class PeriodicBitField<T, typename std::enable_if<std::is_base_of<AbstactBitField, T>::value, void>::type >
        : public PeriodicBitFieldBase<T>
{
public:
    template<typename ValueType>
    PeriodicBitField(Field<ValueType>& numberOf_field, int offset = 0)
        : PeriodicBitFieldBase<T>(numberOf_field, offset)
    {
    }
    
    template<typename ValueType, uint8_t FieldLenght>
    PeriodicBitField(BitField<ValueType, FieldLenght>& numberOf_field, int offset = 0)
        : PeriodicBitFieldBase<T>(numberOf_field, offset)
    {
    }

    PeriodicBitField(int numberOf, int offset = 0)
        : PeriodicBitFieldBase<T>(numberOf, offset)
    {
    }
    
    int bitlength() const override
    {
        T t;
        return t.bitlength() * this->size();
    }
    
    void serialize(BitStream& bitstream) const override
    {
        for (auto & it : (*this))
            bitstream << it;
    }
    
    virtual void deserialize(BitStream& bitstream)
    {
        auto val = this->numberOf.value() + this->offset;
        if (val < 0)
            return;
        
        this->resize(val);
        for (auto & it : (*this))
            bitstream >> it;
    }
};

template <typename T>
class PeriodicBitField<T, typename std::enable_if<std::is_fundamental<T>::value, void>::type >
        : public PeriodicBitFieldBase<T>
{
public:
    template<typename ValueType>
    PeriodicBitField(Field<ValueType>& numberOf_field, int offset = 0)
        : PeriodicBitFieldBase<T>(numberOf_field, offset)
    {
    }
    
    template<typename ValueType, uint8_t FieldLenght>
    PeriodicBitField(BitField<ValueType, FieldLenght>& numberOf_field, int offset = 0)
        : PeriodicBitFieldBase<T>(numberOf_field, offset)
    {
    }

    PeriodicBitField(int numberOf, int offset = 0)
        : PeriodicBitFieldBase<T>(numberOf, offset)
    {
    }
    
    int bitlength() const override
    {
        return static_cast<int>(sizeof(T) * 8 * this->size());
    }
    
    BitStream& serialize(BitStream& stream) const override
    {
        for (auto & it : (*this))
            stream.operator << <T>(it);
        return stream;
    }
    
    BitStream& deserialize(BitStream& stream) override
    {
        auto val = this->numberOf.value() + this->offset;
        if (val < 0)
            return stream;
        
        this->resize(val);
        for (auto & it : (*this))
            stream.operator >> <T>(it);
        return stream;
    }
};


template <typename T>
class PeriodicBitField<T, typename std::enable_if<std::is_base_of<Codogramm, T>::value, void>::type >
        : public PeriodicBitFieldBase<T>
{
public:
    template<typename ValueType>
    PeriodicBitField(Field<ValueType>& numberOf_field, int offset = 0)
        : PeriodicBitFieldBase<T>(numberOf_field, offset)
    {
    }
    
    template<typename ValueType, uint8_t FieldLenght>
    PeriodicBitField(BitField<ValueType, FieldLenght>& numberOf_field, int offset = 0)
        : PeriodicBitFieldBase<T>(numberOf_field, offset)
    {
    }

    PeriodicBitField(int numberOf, int offset = 0)
        : PeriodicBitFieldBase<T>(numberOf, offset)
    {
    }
    
    int bitlength() const override
    {
        int len = 0;
        for (auto & it : (*this))
            len += it.bitlength();
        return len;
    }
    
    BitStream& serialize(BitStream& stream) const override
    {
        for (auto & it : (*this))
            stream << it;
        return stream;
    }
    
    BitStream& deserialize(BitStream& stream) override
    {
        auto val = this->numberOf.value() + this->offset;
        if (val < 0)
            return stream;
        
        this->resize(val);
        for (auto & it : (*this))
            stream >> it;
        return stream;
    }
};
