#include "CarAssembler.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <thread>
#include <chrono>

#define CLEAR_SCREEN "\033[H\033[2J"

static const int SELECTION_DELAY_MS     = 800;
static const int RESULT_DELAY_MS        = 2000;
static const int TEST_PROGRESS_DELAY_MS = 1500;

static void delay(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static bool parseInput(const std::string& line, int& answer)
{
    try
    {
        size_t pos;
        answer = std::stoi(line, &pos);
        if (pos != line.size())
            throw std::invalid_argument("");
        return true;
    }
    catch (const std::exception&)
    {
        printf("ERROR :: 숫자만 입력 가능\n");
        return false;
    }
}

static void applyCarTypeSelection(int answer, CarConfig& config)
{
    config.carType = static_cast<CarType>(answer);
    static const char* names[] = { "", "Sedan", "SUV", "Truck" };
    printf("차량 타입으로 %s을 선택하셨습니다.\n", names[answer]);
}

static void applyEngineSelection(int answer, CarConfig& config)
{
    config.engine = static_cast<Engine>(answer);
    static const char* names[] = { "", "GM", "TOYOTA", "WIA" };
    if (answer >= 1 && answer <= 3)
        printf("%s 엔진을 선택하셨습니다.\n", names[answer]);
}

static void applyBrakeSystemSelection(int answer, CarConfig& config)
{
    config.brakeSystem = static_cast<BrakeSystem>(answer);
    static const char* names[] = { "", "MANDO", "CONTINENTAL", "BOSCH" };
    printf("%s 제동장치를 선택하셨습니다.\n", names[answer]);
}

static void applySteeringSystemSelection(int answer, CarConfig& config)
{
    config.steeringSystem = static_cast<SteeringSystem>(answer);
    static const char* names[] = { "", "BOSCH", "MOBIS" };
    printf("%s 조향장치를 선택하셨습니다.\n", names[answer]);
}

static void runCar(const Car& car)
{
    CarValidator validator;
    if (!validator.isValid(car))
    {
        printf("자동차가 동작되지 않습니다\n");
    }
    else if (car.getEngine()->engineType() == Engine::BROKEN)
    {
        printf("엔진이 고장나있습니다.\n");
        printf("자동차가 움직이지 않습니다.\n");
    }
    else
    {
        car.printInfo();
        printf("자동차가 동작됩니다.\n");
    }
    delay(RESULT_DELAY_MS);
}

static void testCar(const Car& car)
{
    CarValidator validator;
    printf("Test...\n");
    delay(TEST_PROGRESS_DELAY_MS);
    if (!validator.isValid(car))
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
    else
        printf("자동차 부품 조합 테스트 결과 : PASS\n");
    delay(RESULT_DELAY_MS);
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

        if (line == "exit") { printf("바이바이\n"); break; }

        int answer;
        if (!parseInput(line, answer))        { delay(SELECTION_DELAY_MS); continue; }
        if (!validateInput(step, answer))     { delay(SELECTION_DELAY_MS); continue; }

        if (answer == 0 && step == Run_Test)  { step = CarType_Q; continue; }
        if (answer == 0 && step >= 1)         { step -= 1;        continue; }

        switch (step)
        {
        case CarType_Q:
            applyCarTypeSelection(answer, config);
            delay(SELECTION_DELAY_MS);
            step = Engine_Q;
            break;
        case Engine_Q:
            applyEngineSelection(answer, config);
            delay(SELECTION_DELAY_MS);
            step = BrakeSystem_Q;
            break;
        case BrakeSystem_Q:
            applyBrakeSystemSelection(answer, config);
            delay(SELECTION_DELAY_MS);
            step = SteeringSystem_Q;
            break;
        case SteeringSystem_Q:
            applySteeringSystemSelection(answer, config);
            delay(SELECTION_DELAY_MS);
            step = Run_Test;
            break;
        case Run_Test:
            if (answer == 1) runCar(buildCar(config));
            else             testCar(buildCar(config));
            break;
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
    std::map<Engine, IEngine*> engineMap = {
        { Engine::GM,     &m_gmEngine     },
        { Engine::TOYOTA, &m_toyotaEngine },
        { Engine::WIA,    &m_wiaEngine    },
        { Engine::BROKEN, &m_brokenEngine },
    };
    std::map<BrakeSystem, IBrakeSystem*> brakeMap = {
        { BrakeSystem::MANDO,       &m_mandoBrake       },
        { BrakeSystem::CONTINENTAL, &m_continentalBrake },
        { BrakeSystem::BOSCH,       &m_boschBrake       },
    };
    std::map<SteeringSystem, ISteeringSystem*> steeringMap = {
        { SteeringSystem::BOSCH, &m_boschSteering },
        { SteeringSystem::MOBIS, &m_mobisSteering },
    };
    return Car(config.carType, engineMap.at(config.engine), brakeMap.at(config.brakeSystem), steeringMap.at(config.steeringSystem));
}
