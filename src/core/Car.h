#pragma once
#include "Assemble.h"
#include "../parts/IEngine.h"
#include "../parts/IBrakeSystem.h"
#include "../parts/ISteeringSystem.h"

class Car
{
public:
    Car(CarType type, IEngine* engine, IBrakeSystem* brakeSystem, ISteeringSystem* steeringSystem);

    CarType          getType()           const;
    IEngine*         getEngine()         const;
    IBrakeSystem*    getBrakeSystem()    const;
    ISteeringSystem* getSteeringSystem() const;

    void printInfo() const;

private:
    CarType          m_type;
    IEngine*         m_engine;
    IBrakeSystem*    m_brakeSystem;
    ISteeringSystem* m_steeringSystem;
};
