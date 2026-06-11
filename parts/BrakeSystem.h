#pragma once
#include "IBrakeSystem.h"

class MandoBrake : public IBrakeSystem
{
public:
    std::string name() const override { return "Mando"; }
    BrakeSystem brakeSystemType() const override { return BrakeSystem::MANDO; }
};

class ContinentalBrake : public IBrakeSystem
{
public:
    std::string name() const override { return "Continental"; }
    BrakeSystem brakeSystemType() const override { return BrakeSystem::CONTINENTAL; }
};

class BoschBrake : public IBrakeSystem
{
public:
    std::string name() const override { return "Bosch"; }
    BrakeSystem brakeSystemType() const override { return BrakeSystem::BOSCH; }
};
