#include "Car.h"
#include <stdio.h>

Car::Car(CarType type, IEngine* engine, IBrakeSystem* brakeSystem, ISteeringSystem* steeringSystem)
    : m_type(type), m_engine(engine), m_brakeSystem(brakeSystem), m_steeringSystem(steeringSystem)
{}

CarType          Car::getType()           const { return m_type; }
IEngine*         Car::getEngine()         const { return m_engine; }
IBrakeSystem*    Car::getBrakeSystem()    const { return m_brakeSystem; }
ISteeringSystem* Car::getSteeringSystem() const { return m_steeringSystem; }

void Car::printInfo() const
{
    static const char* carTypeNames[] = { "", "Sedan", "SUV", "Truck" };
    printf("Car Type : %s\n", carTypeNames[static_cast<int>(m_type)]);
    printf("Engine : %s\n", m_engine->name().c_str());
    printf("Brake System : %s\n", m_brakeSystem->name().c_str());
    printf("SteeringSystem : %s\n", m_steeringSystem->name().c_str());
}
