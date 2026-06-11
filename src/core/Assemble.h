#pragma once

enum class CarType
{
    SEDAN = 1,
    SUV,
    TRUCK
};

enum class Engine
{
    GM = 1,
    TOYOTA,
    WIA,
    BROKEN
};

enum class BrakeSystem
{
    MANDO = 1,
    CONTINENTAL,
    BOSCH
};

enum class SteeringSystem
{
    BOSCH = 1,
    MOBIS
};

struct CarConfig
{
    CarType        carType        = CarType::SEDAN;
    Engine         engine         = Engine::GM;
    BrakeSystem    brakeSystem    = BrakeSystem::MANDO;
    SteeringSystem steeringSystem = SteeringSystem::BOSCH;
};

bool isValidCheck(const CarConfig& config);
