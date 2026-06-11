#pragma once
#include "../core/Assemble.h"
#include <string>

class ISteeringSystem
{
public:
    virtual ~ISteeringSystem() = default;
    virtual std::string name() const = 0;
    virtual SteeringSystem steeringSystemType() const = 0;
};
