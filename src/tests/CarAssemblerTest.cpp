#include "gmock/gmock.h"
#include "../core/CarAssembler.h"

class TestableCarAssembler : public CarAssembler
{
public:
    using CarAssembler::validateInput;
    using CarAssembler::buildCar;
    using CarAssembler::CarType_Q;
    using CarAssembler::Engine_Q;
    using CarAssembler::BrakeSystem_Q;
    using CarAssembler::SteeringSystem_Q;
    using CarAssembler::Run_Test;
};

class CarAssemblerTest : public ::testing::Test
{
protected:
    TestableCarAssembler assembler;
};

// =====================================================================
// validateInput — CarType_Q (유효 범위 1~3)
// =====================================================================
TEST_F(CarAssemblerTest, ValidateInput_CarType_Valid)
{
    EXPECT_TRUE(assembler.validateInput(assembler.CarType_Q, 1));
    EXPECT_TRUE(assembler.validateInput(assembler.CarType_Q, 2));
    EXPECT_TRUE(assembler.validateInput(assembler.CarType_Q, 3));
}
TEST_F(CarAssemblerTest, ValidateInput_CarType_Invalid)
{
    EXPECT_FALSE(assembler.validateInput(assembler.CarType_Q, 0));
    EXPECT_FALSE(assembler.validateInput(assembler.CarType_Q, 4));
}

// =====================================================================
// validateInput — Engine_Q (유효 범위 0~4)
// =====================================================================
TEST_F(CarAssemblerTest, ValidateInput_Engine_Valid)
{
    EXPECT_TRUE(assembler.validateInput(assembler.Engine_Q, 0));
    EXPECT_TRUE(assembler.validateInput(assembler.Engine_Q, 1));
    EXPECT_TRUE(assembler.validateInput(assembler.Engine_Q, 4));
}
TEST_F(CarAssemblerTest, ValidateInput_Engine_Invalid)
{
    EXPECT_FALSE(assembler.validateInput(assembler.Engine_Q, -1));
    EXPECT_FALSE(assembler.validateInput(assembler.Engine_Q, 5));
}

// =====================================================================
// validateInput — BrakeSystem_Q (유효 범위 0~3)
// =====================================================================
TEST_F(CarAssemblerTest, ValidateInput_BrakeSystem_Valid)
{
    EXPECT_TRUE(assembler.validateInput(assembler.BrakeSystem_Q, 0));
    EXPECT_TRUE(assembler.validateInput(assembler.BrakeSystem_Q, 1));
    EXPECT_TRUE(assembler.validateInput(assembler.BrakeSystem_Q, 3));
}
TEST_F(CarAssemblerTest, ValidateInput_BrakeSystem_Invalid)
{
    EXPECT_FALSE(assembler.validateInput(assembler.BrakeSystem_Q, -1));
    EXPECT_FALSE(assembler.validateInput(assembler.BrakeSystem_Q, 4));
}

// =====================================================================
// validateInput — SteeringSystem_Q (유효 범위 0~2)
// =====================================================================
TEST_F(CarAssemblerTest, ValidateInput_SteeringSystem_Valid)
{
    EXPECT_TRUE(assembler.validateInput(assembler.SteeringSystem_Q, 0));
    EXPECT_TRUE(assembler.validateInput(assembler.SteeringSystem_Q, 1));
    EXPECT_TRUE(assembler.validateInput(assembler.SteeringSystem_Q, 2));
}
TEST_F(CarAssemblerTest, ValidateInput_SteeringSystem_Invalid)
{
    EXPECT_FALSE(assembler.validateInput(assembler.SteeringSystem_Q, -1));
    EXPECT_FALSE(assembler.validateInput(assembler.SteeringSystem_Q, 3));
}

// =====================================================================
// validateInput — Run_Test (유효 범위 0~2)
// =====================================================================
TEST_F(CarAssemblerTest, ValidateInput_RunTest_Valid)
{
    EXPECT_TRUE(assembler.validateInput(assembler.Run_Test, 0));
    EXPECT_TRUE(assembler.validateInput(assembler.Run_Test, 1));
    EXPECT_TRUE(assembler.validateInput(assembler.Run_Test, 2));
}
TEST_F(CarAssemblerTest, ValidateInput_RunTest_Invalid)
{
    EXPECT_FALSE(assembler.validateInput(assembler.Run_Test, -1));
    EXPECT_FALSE(assembler.validateInput(assembler.Run_Test, 3));
}

// =====================================================================
// buildCar — 차량 타입 매핑
// =====================================================================
TEST_F(CarAssemblerTest, BuildCar_CarType_Sedan)
{
    CarConfig config{ CarType::SEDAN, Engine::GM, BrakeSystem::MANDO, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getType(), CarType::SEDAN);
}
TEST_F(CarAssemblerTest, BuildCar_CarType_Suv)
{
    CarConfig config{ CarType::SUV, Engine::GM, BrakeSystem::MANDO, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getType(), CarType::SUV);
}
TEST_F(CarAssemblerTest, BuildCar_CarType_Truck)
{
    CarConfig config{ CarType::TRUCK, Engine::GM, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getType(), CarType::TRUCK);
}

// =====================================================================
// buildCar — 엔진 매핑
// =====================================================================
TEST_F(CarAssemblerTest, BuildCar_Engine_GM)
{
    CarConfig config{ CarType::SEDAN, Engine::GM, BrakeSystem::MANDO, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getEngine()->engineType(), Engine::GM);
}
TEST_F(CarAssemblerTest, BuildCar_Engine_Toyota)
{
    CarConfig config{ CarType::SEDAN, Engine::TOYOTA, BrakeSystem::MANDO, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getEngine()->engineType(), Engine::TOYOTA);
}
TEST_F(CarAssemblerTest, BuildCar_Engine_Wia)
{
    CarConfig config{ CarType::SEDAN, Engine::WIA, BrakeSystem::MANDO, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getEngine()->engineType(), Engine::WIA);
}
TEST_F(CarAssemblerTest, BuildCar_Engine_Broken)
{
    CarConfig config{ CarType::SEDAN, Engine::BROKEN, BrakeSystem::MANDO, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getEngine()->engineType(), Engine::BROKEN);
}

// =====================================================================
// buildCar — 제동장치 매핑
// =====================================================================
TEST_F(CarAssemblerTest, BuildCar_Brake_Mando)
{
    CarConfig config{ CarType::SEDAN, Engine::GM, BrakeSystem::MANDO, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getBrakeSystem()->brakeSystemType(), BrakeSystem::MANDO);
}
TEST_F(CarAssemblerTest, BuildCar_Brake_Continental)
{
    CarConfig config{ CarType::SEDAN, Engine::GM, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getBrakeSystem()->brakeSystemType(), BrakeSystem::CONTINENTAL);
}
TEST_F(CarAssemblerTest, BuildCar_Brake_Bosch)
{
    CarConfig config{ CarType::SEDAN, Engine::GM, BrakeSystem::BOSCH, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getBrakeSystem()->brakeSystemType(), BrakeSystem::BOSCH);
}

// =====================================================================
// buildCar — 조향장치 매핑
// =====================================================================
TEST_F(CarAssemblerTest, BuildCar_Steering_Bosch)
{
    CarConfig config{ CarType::SEDAN, Engine::GM, BrakeSystem::MANDO, SteeringSystem::BOSCH };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getSteeringSystem()->steeringSystemType(), SteeringSystem::BOSCH);
}
TEST_F(CarAssemblerTest, BuildCar_Steering_Mobis)
{
    CarConfig config{ CarType::SEDAN, Engine::GM, BrakeSystem::MANDO, SteeringSystem::MOBIS };
    Car car = assembler.buildCar(config);
    EXPECT_EQ(car.getSteeringSystem()->steeringSystemType(), SteeringSystem::MOBIS);
}
