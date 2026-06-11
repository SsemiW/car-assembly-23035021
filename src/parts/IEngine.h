#pragma once
#include "../core/Assemble.h"
#include <string>

class IEngine
{
public:
    virtual ~IEngine() = default;
    virtual std::string name() const = 0;
    virtual Engine engineType() const = 0;
};
