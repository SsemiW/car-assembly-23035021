#ifdef _DEBUG

#include "gmock/gmock.h"

int main()
{
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CLEAR_SCREEN "\033[H\033[2J"

enum QuestionType
{
    CarType_Q,
    Engine_Q,
    brakeSystem_Q,
    SteeringSystem_Q,
    Run_Test,
};

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

void selectCarType(int answer, CarConfig& config);
void selectEngine(int answer, CarConfig& config);
void selectbrakeSystem(int answer, CarConfig& config);
void selectSteeringSystem(int answer, CarConfig& config);
void runProducedCar(const CarConfig& config);
void testProducedCar(const CarConfig& config);
int isValidCheck(const CarConfig& config);
void delay(int ms);

void delay(int ms)
{
    volatile int sum = 0;
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            for (int t = 0; t < ms; t++)
            {
                sum++;
            }
        }
    }
}

int main()
{
    char buf[100];
    int step = CarType_Q;
    CarConfig config;

    while (1)
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
        else if (step == brakeSystem_Q)
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

        printf("INPUT > ");
        fgets(buf, sizeof(buf), stdin);

        // 엔터 개행문자 제거
        char* context = nullptr;
        strtok_s(buf, "\r", &context);
        strtok_s(buf, "\n", &context);

        if (!strcmp(buf, "exit"))
        {
            printf("바이바이\n");
            break;
        }

        // 숫자로 된 대답인지 확인
        char* checkNumber;
        int answer = strtol(buf, &checkNumber, 10); // 문자열을 10진수로 변환

        // 입력받은 문자가 숫자가 아니라면
        if (*checkNumber != '\0')
        {
            printf("ERROR :: 숫자만 입력 가능\n");
            delay(800);
            continue;
        }

        if (step == CarType_Q && !(answer >= 1 && answer <= 3))
        {
            printf("ERROR :: 차량 타입은 1 ~ 3 범위만 선택 가능\n");
            delay(800);
            continue;
        }

        if (step == Engine_Q && !(answer >= 0 && answer <= 4))
        {
            printf("ERROR :: 엔진은 1 ~ 4 범위만 선택 가능\n");
            delay(800);
            continue;
        }

        if (step == brakeSystem_Q && !(answer >= 0 && answer <= 3))
        {
            printf("ERROR :: 제동장치는 1 ~ 3 범위만 선택 가능\n");
            delay(800);
            continue;
        }

        if (step == SteeringSystem_Q && !(answer >= 0 && answer <= 2))
        {
            printf("ERROR :: 조향장치는 1 ~ 2 범위만 선택 가능\n");
            delay(800);
            continue;
        }

        if (step == Run_Test && !(answer >= 0 && answer <= 2))
        {
            printf("ERROR :: Run 또는 Test 중 하나를 선택 필요\n");
            delay(800);
            continue;
        }

        // 처음으로 돌아가기
        if (answer == 0 && step == Run_Test)
        {
            step = CarType_Q;
            continue;
        }

        // 이전으로 돌아가기
        if (answer == 0 && step >= 1)
        {
            step -= 1;
            continue;
        }

        if (step == CarType_Q)
        {
            selectCarType(answer, config);
            delay(800);
            step = Engine_Q;
        }
        else if (step == Engine_Q)
        {
            selectEngine(answer, config);
            delay(800);
            step = brakeSystem_Q;
        }
        else if (step == brakeSystem_Q)
        {
            selectbrakeSystem(answer, config);
            delay(800);
            step = SteeringSystem_Q;
        }
        else if (step == SteeringSystem_Q)
        {
            selectSteeringSystem(answer, config);
            delay(800);
            step = Run_Test;
        }
        else if (step == Run_Test && answer == 1)
        {
            runProducedCar(config);
            delay(2000);
        }
        else if (step == Run_Test && answer == 2)
        {
            printf("Test...\n");
            delay(1500);
            testProducedCar(config);
            delay(2000);
        }
    }
}

void selectCarType(int answer, CarConfig& config)
{
    config.carType = static_cast<CarType>(answer);
    if (answer == 1)
        printf("차량 타입으로 Sedan을 선택하셨습니다.\n");
    if (answer == 2)
        printf("차량 타입으로 SUV을 선택하셨습니다.\n");
    if (answer == 3)
        printf("차량 타입으로 Truck을 선택하셨습니다.\n");
}

void selectEngine(int answer, CarConfig& config)
{
    config.engine = static_cast<Engine>(answer);
    if (answer == 1)
        printf("GM 엔진을 선택하셨습니다.\n");
    if (answer == 2)
        printf("TOYOTA 엔진을 선택하셨습니다.\n");
    if (answer == 3)
        printf("WIA 엔진을 선택하셨습니다.\n");
}

void selectbrakeSystem(int answer, CarConfig& config)
{
    config.brakeSystem = static_cast<BrakeSystem>(answer);
    if (answer == 1)
        printf("MANDO 제동장치를 선택하셨습니다.\n");
    if (answer == 2)
        printf("CONTINENTAL 제동장치를 선택하셨습니다.\n");
    if (answer == 3)
        printf("BOSCH 제동장치를 선택하셨습니다.\n");
}

void selectSteeringSystem(int answer, CarConfig& config)
{
    config.steeringSystem = static_cast<SteeringSystem>(answer);
    if (answer == 1)
        printf("BOSCH 조향장치를 선택하셨습니다.\n");
    if (answer == 2)
        printf("MOBIS 조향장치를 선택하셨습니다.\n");
}

int isValidCheck(const CarConfig& config)
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
    return true;
}

void runProducedCar(const CarConfig& config)
{
    if (isValidCheck(config) == false)
    {
        printf("자동차가 동작되지 않습니다\n");
    }
    else
    {
        if (config.engine == Engine::BROKEN)
        {
            printf("엔진이 고장나있습니다.\n");
            printf("자동차가 움직이지 않습니다.\n");
        }
        else
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

            printf("자동차가 동작됩니다.\n");
        }
    }
}

void testProducedCar(const CarConfig& config)
{
    if (config.carType == CarType::SEDAN && config.brakeSystem == BrakeSystem::CONTINENTAL)
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Sedan에는 Continental제동장치 사용 불가\n");
    }
    else if (config.carType == CarType::SUV && config.engine == Engine::TOYOTA)
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("SUV에는 TOYOTA엔진 사용 불가\n");
    }
    else if (config.carType == CarType::TRUCK && config.engine == Engine::WIA)
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Truck에는 WIA엔진 사용 불가\n");
    }
    else if (config.carType == CarType::TRUCK && config.brakeSystem == BrakeSystem::MANDO)
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Truck에는 Mando제동장치 사용 불가\n");
    }
    else if (config.brakeSystem == BrakeSystem::BOSCH && config.steeringSystem != SteeringSystem::BOSCH)
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Bosch제동장치에는 Bosch조향장치 이외 사용 불가\n");
    }
    else
    {
        printf("자동차 부품 조합 테스트 결과 : PASS\n");
    }
}

#endif