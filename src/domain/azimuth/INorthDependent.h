#pragma once

class INorthDependent
{
public:
    virtual ~INorthDependent() = default;

    virtual void onNorth(int turnoverNumber) = 0;
};
