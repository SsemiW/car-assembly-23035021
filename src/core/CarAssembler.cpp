#include "CarAssembler.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#define CLEAR_SCREEN "\033[H\033[2J"

static void delay(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void CarAssembler::run()
{
    std::string line;
    int step = CarType_Q;
    CarConfig config;

    while (true)
    {
        printMenu(step);

        printf("INPUT > ");
        std::getline(std::cin, line);
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line == "exit")
        {
            printf("바이바이\n");
            break;
        }

        int answer;
        try
        {
            size_t pos;
            answer = std::stoi(line, &pos);
            if (pos != line.size())
                throw std::invalid_argument("");
        }
        catch (const std::exception&)
        {
            printf("ERROR :: 숫자만 입력 가능\n");
            delay(800);
            continue;
        }

        if (!validateInput(step, answer))
        {
            delay(800);
            continue;
        }

        if (answer == 0 && step == Run_Test)
        {
            step = CarType_Q;
            continue;
        }

        if (answer == 0 && step >= 1)
        {
            step -= 1;
            continue;
        }

        if (step == CarType_Q)
        {
            config.carType = static_cast<CarType>(answer);
            if (answer == 1) printf("차량 타입으로 Sedan을 선택하셨습니다.\n");
            if (answer == 2) printf("차량 타입으로 SUV을 선택하셨습니다.\n");
            if (answer == 3) printf("차량 타입으로 Truck을 선택하셨습니다.\n");
            delay(800);
            step = Engine_Q;
        }
        else if (step == Engine_Q)
        {
            config.engine = static_cast<Engine>(answer);
            if (answer == 1) printf("GM 엔진을 선택하셨습니다.\n");
            if (answer == 2) printf("TOYOTA 엔진을 선택하셨습니다.\n");
            if (answer == 3) printf("WIA 엔진을 선택하셨습니다.\n");
            delay(800);
            step = BrakeSystem_Q;
        }
        else if (step == BrakeSystem_Q)
        {
            config.brakeSystem = static_cast<BrakeSystem>(answer);
            if (answer == 1) printf("MANDO 제동장치를 선택하셨습니다.\n");
            if (answer == 2) printf("CONTINENTAL 제동장치를 선택하셨습니다.\n");
            if (answer == 3) printf("BOSCH 제동장치를 선택하셨습니다.\n");
            delay(800);
            step = SteeringSystem_Q;
        }
        else if (step == SteeringSystem_Q)
        {
            config.steeringSystem = static_cast<SteeringSystem>(answer);
            if (answer == 1) printf("BOSCH 조향장치를 선택하셨습니다.\n");
            if (answer == 2) printf("MOBIS 조향장치를 선택하셨습니다.\n");
            delay(800);
            step = Run_Test;
        }
        else if (step == Run_Test && answer == 1)
        {
            Car car = buildCar(config);
            CarValidator validator;
            if (!validator.isValid(car))
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
                car.printInfo();
                printf("자동차가 동작됩니다.\n");
            }
            delay(2000);
        }
        else if (step == Run_Test && answer == 2)
        {
            Car car = buildCar(config);
            CarValidator validator;
            printf("Test...\n");
            delay(1500);
            if (!validator.isValid(car))
                printf("자동차 부품 조합 테스트 결과 : FAIL\n");
            else
                printf("자동차 부품 조합 테스트 결과 : PASS\n");
            delay(2000);
        }
    }
}

void CarAssembler::printMenu(int step) const
{
    if (step == CarType_Q)
    {
        printf(CLEAR_SCREEN);
        printf("        ______________\n");
        printf("       /|            | \n");
        printf("  ____/_|_____________|____\n");
        printf(" |                      O  |\n");
        printf(" '-(@)----------------(@)--'\n");
        printf("===============================\n");
        printf("어떤 차량 타입을 선택할까요?\n");
        printf("1. Sedan\n");
        printf("2. SUV\n");
        printf("3. Truck\n");
    }
    else if (step == Engine_Q)
    {
        printf(CLEAR_SCREEN);
        printf("어떤 엔진을 탑재할까요?\n");
        printf("0. 뒤로가기\n");
        printf("1. GM\n");
        printf("2. TOYOTA\n");
        printf("3. WIA\n");
        printf("4. 고장난 엔진\n");
    }
    else if (step == BrakeSystem_Q)
    {
        printf(CLEAR_SCREEN);
        printf("어떤 제동장치를 선택할까요?\n");
        printf("0. 뒤로가기\n");
        printf("1. MANDO\n");
        printf("2. CONTINENTAL\n");
        printf("3. BOSCH\n");
    }
    else if (step == SteeringSystem_Q)
    {
        printf(CLEAR_SCREEN);
        printf("어떤 조향장치를 선택할까요?\n");
        printf("0. 뒤로가기\n");
        printf("1. BOSCH\n");
        printf("2. MOBIS\n");
    }
    else if (step == Run_Test)
    {
        printf(CLEAR_SCREEN);
        printf("멋진 차량이 완성되었습니다.\n");
        printf("어떤 동작을 할까요?\n");
        printf("0. 처음 화면으로 돌아가기\n");
        printf("1. RUN\n");
        printf("2. Test\n");
    }
    printf("===============================\n");
}

bool CarAssembler::validateInput(int step, int answer) const
{
    if (step == CarType_Q && !(answer >= 1 && answer <= 3))
    {
        printf("ERROR :: 차량 타입은 1 ~ 3 범위만 선택 가능\n");
        return false;
    }
    if (step == Engine_Q && !(answer >= 0 && answer <= 4))
    {
        printf("ERROR :: 엔진은 1 ~ 4 범위만 선택 가능\n");
        return false;
    }
    if (step == BrakeSystem_Q && !(answer >= 0 && answer <= 3))
    {
        printf("ERROR :: 제동장치는 1 ~ 3 범위만 선택 가능\n");
        return false;
    }
    if (step == SteeringSystem_Q && !(answer >= 0 && answer <= 2))
    {
        printf("ERROR :: 조향장치는 1 ~ 2 범위만 선택 가능\n");
        return false;
    }
    if (step == Run_Test && !(answer >= 0 && answer <= 2))
    {
        printf("ERROR :: Run 또는 Test 중 하나를 선택 필요\n");
        return false;
    }
    return true;
}

Car CarAssembler::buildCar(const CarConfig& config)
{
    IEngine* engine = nullptr;
    switch (config.engine)
    {
    case Engine::GM:     engine = &m_gmEngine;     break;
    case Engine::TOYOTA: engine = &m_toyotaEngine; break;
    case Engine::WIA:    engine = &m_wiaEngine;    break;
    case Engine::BROKEN: engine = &m_brokenEngine; break;
    }

    IBrakeSystem* brakeSystem = nullptr;
    switch (config.brakeSystem)
    {
    case BrakeSystem::MANDO:       brakeSystem = &m_mandoBrake;       break;
    case BrakeSystem::CONTINENTAL: brakeSystem = &m_continentalBrake; break;
    case BrakeSystem::BOSCH:       brakeSystem = &m_boschBrake;       break;
    }

    ISteeringSystem* steeringSystem = nullptr;
    switch (config.steeringSystem)
    {
    case SteeringSystem::BOSCH: steeringSystem = &m_boschSteering; break;
    case SteeringSystem::MOBIS: steeringSystem = &m_mobisSteering; break;
    }

    return Car(config.carType, engine, brakeSystem, steeringSystem);
}
