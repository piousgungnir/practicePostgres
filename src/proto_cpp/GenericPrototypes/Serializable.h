#pragma once


class BitStream;


class Serializable
{
public:
    virtual ~Serializable() = default;

    virtual BitStream& serialize(BitStream& stream) const = 0;
    virtual BitStream& deserialize(BitStream& stream) = 0;
};
