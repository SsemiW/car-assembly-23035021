#pragma once
#include "ISteeringSystem.h"

class BoschSteering : public ISteeringSystem
{
public:
    std::string name() const override { return "Bosch"; }
    SteeringSystem steeringSystemType() const override { return SteeringSystem::BOSCH; }
};

class MobisSteering : public ISteeringSystem
{
public:
    std::string name() const override { return "Mobis"; }
    SteeringSystem steeringSystemType() const override { return SteeringSystem::MOBIS; }
};
