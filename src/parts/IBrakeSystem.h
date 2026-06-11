#pragma once
#include "../core/assemble.h"
#include <string>

class IBrakeSystem
{
public:
    virtual ~IBrakeSystem() = default;
    virtual std::string name() const = 0;
    virtual BrakeSystem brakeSystemType() const = 0;
};
