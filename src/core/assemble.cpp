#include "assemble.h"
#include <stdio.h>

bool isValidCheck(const CarConfig& config)
{
    if (config.carType == CarType::SEDAN && config.brakeSystem == BrakeSystem::CONTINENTAL)
    {
        return false;
    }
    else if (config.carType == CarType::SUV && config.engine == Engine::TOYOTA)
    {
        return false;
    }
    else if (config.carType == CarType::TRUCK && config.engine == Engine::WIA)
    {
        return false;
    }
    else if (config.carType == CarType::TRUCK && config.brakeSystem == BrakeSystem::MANDO)
    {
        return false;
    }
    else if (config.brakeSystem == BrakeSystem::BOSCH && config.steeringSystem != SteeringSystem::BOSCH)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void printCarInfo(const CarConfig& config)
{
    if (config.carType == CarType::SEDAN)
        printf("Car Type : Sedan\n");
    if (config.carType == CarType::SUV)
        printf("Car Type : SUV\n");
    if (config.carType == CarType::TRUCK)
        printf("Car Type : Truck\n");
    if (config.engine == Engine::GM)
        printf("Engine : GM\n");
    if (config.engine == Engine::TOYOTA)
        printf("Engine : TOYOTA\n");
    if (config.engine == Engine::WIA)
        printf("Engine : WIA\n");
    if (config.brakeSystem == BrakeSystem::MANDO)
        printf("Brake System : Mando\n");
    if (config.brakeSystem == BrakeSystem::CONTINENTAL)
        printf("Brake System : Continental\n");
    if (config.brakeSystem == BrakeSystem::BOSCH)
        printf("Brake System : Bosch\n");
    if (config.steeringSystem == SteeringSystem::BOSCH)
        printf("SteeringSystem : Bosch\n");
    if (config.steeringSystem == SteeringSystem::MOBIS)
        printf("SteeringSystem : Mobis\n");
}

void runProducedCar(const CarConfig& config)
{
    if (!isValidCheck(config))
    {
        printf("자동차가 동작되지 않습니다\n");
    }
    else if (config.engine == Engine::BROKEN)
    {
        printf("엔진이 고장나있습니다.\n");
        printf("자동차가 움직이지 않습니다.\n");
    }
    else
    {
        printCarInfo(config);
        printf("자동차가 동작됩니다.\n");
    }
}

void testProducedCar(const CarConfig& config)
{
    if (!isValidCheck(config))
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
    else
        printf("자동차 부품 조합 테스트 결과 : PASS\n");
}
