#include "gmock/gmock.h"
#include "../core/CarValidator.h"
#include "../core/Car.h"
#include "../parts/Engine.h"
#include "../parts/BrakeSystem.h"
#include "../parts/SteeringSystem.h"

class CarValidatorTest : public ::testing::Test
{
protected:
    GmEngine         gmEngine;
    ToyotaEngine     toyotaEngine;
    WiaEngine        wiaEngine;
    BrokenEngine     brokenEngine;
    MandoBrake       mandoBrake;
    ContinentalBrake continentalBrake;
    BoschBrake       boschBrake;
    BoschSteering    boschSteering;
    MobisSteering    mobisSteering;

    CarValidator validator;
};

// =====================================================================
// C1: SEDAN + CONTINENTAL (엔진·조향 무관)
// =====================================================================
TEST_F(CarValidatorTest, C1_Sedan_GM_Continental_Bosch_Fail)       { Car c(CarType::SEDAN, &gmEngine,     &continentalBrake, &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C1_Sedan_GM_Continental_Mobis_Fail)       { Car c(CarType::SEDAN, &gmEngine,     &continentalBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C1_Sedan_Toyota_Continental_Bosch_Fail)   { Car c(CarType::SEDAN, &toyotaEngine, &continentalBrake, &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C1_Sedan_Toyota_Continental_Mobis_Fail)   { Car c(CarType::SEDAN, &toyotaEngine, &continentalBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C1_Sedan_Wia_Continental_Bosch_Fail)      { Car c(CarType::SEDAN, &wiaEngine,    &continentalBrake, &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C1_Sedan_Wia_Continental_Mobis_Fail)      { Car c(CarType::SEDAN, &wiaEngine,    &continentalBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }

// =====================================================================
// C2: SUV + TOYOTA (제동·조향 무관)
// =====================================================================
TEST_F(CarValidatorTest, C2_Suv_Toyota_Mando_Bosch_Fail)           { Car c(CarType::SUV, &toyotaEngine, &mandoBrake,       &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C2_Suv_Toyota_Mando_Mobis_Fail)           { Car c(CarType::SUV, &toyotaEngine, &mandoBrake,       &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C2_Suv_Toyota_Continental_Bosch_Fail)     { Car c(CarType::SUV, &toyotaEngine, &continentalBrake, &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C2_Suv_Toyota_Continental_Mobis_Fail)     { Car c(CarType::SUV, &toyotaEngine, &continentalBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C2_Suv_Toyota_Bosch_Bosch_Fail)           { Car c(CarType::SUV, &toyotaEngine, &boschBrake,       &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C2_Suv_Toyota_Bosch_Mobis_Fail)           { Car c(CarType::SUV, &toyotaEngine, &boschBrake,       &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }

// =====================================================================
// C3: TRUCK + WIA (제동·조향 무관)
// =====================================================================
TEST_F(CarValidatorTest, C3_Truck_Wia_Mando_Bosch_Fail)            { Car c(CarType::TRUCK, &wiaEngine, &mandoBrake,       &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C3_Truck_Wia_Mando_Mobis_Fail)            { Car c(CarType::TRUCK, &wiaEngine, &mandoBrake,       &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C3_Truck_Wia_Continental_Bosch_Fail)      { Car c(CarType::TRUCK, &wiaEngine, &continentalBrake, &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C3_Truck_Wia_Continental_Mobis_Fail)      { Car c(CarType::TRUCK, &wiaEngine, &continentalBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C3_Truck_Wia_Bosch_Bosch_Fail)            { Car c(CarType::TRUCK, &wiaEngine, &boschBrake,       &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C3_Truck_Wia_Bosch_Mobis_Fail)            { Car c(CarType::TRUCK, &wiaEngine, &boschBrake,       &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }

// =====================================================================
// C4: TRUCK + MANDO (WIA 엔진 제외)
// =====================================================================
TEST_F(CarValidatorTest, C4_Truck_GM_Mando_Bosch_Fail)             { Car c(CarType::TRUCK, &gmEngine,     &mandoBrake, &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C4_Truck_GM_Mando_Mobis_Fail)             { Car c(CarType::TRUCK, &gmEngine,     &mandoBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C4_Truck_Toyota_Mando_Bosch_Fail)         { Car c(CarType::TRUCK, &toyotaEngine, &mandoBrake, &boschSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C4_Truck_Toyota_Mando_Mobis_Fail)         { Car c(CarType::TRUCK, &toyotaEngine, &mandoBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }

// =====================================================================
// C5: BOSCH brake + MOBIS steering (C2/C3 중복 제외)
// =====================================================================
TEST_F(CarValidatorTest, C5_Sedan_GM_Bosch_Mobis_Fail)             { Car c(CarType::SEDAN, &gmEngine,     &boschBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C5_Sedan_Toyota_Bosch_Mobis_Fail)         { Car c(CarType::SEDAN, &toyotaEngine, &boschBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C5_Sedan_Wia_Bosch_Mobis_Fail)            { Car c(CarType::SEDAN, &wiaEngine,    &boschBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C5_Suv_GM_Bosch_Mobis_Fail)               { Car c(CarType::SUV,   &gmEngine,     &boschBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C5_Suv_Wia_Bosch_Mobis_Fail)              { Car c(CarType::SUV,   &wiaEngine,    &boschBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C5_Truck_GM_Bosch_Mobis_Fail)             { Car c(CarType::TRUCK, &gmEngine,     &boschBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }
TEST_F(CarValidatorTest, C5_Truck_Toyota_Bosch_Mobis_Fail)         { Car c(CarType::TRUCK, &toyotaEngine, &boschBrake, &mobisSteering); EXPECT_FALSE(validator.isValid(c)); }

// =====================================================================
// PASS: 유효 조합
// =====================================================================

// SEDAN
TEST_F(CarValidatorTest, Pass_Sedan_GM_Mando_Bosch)                { Car c(CarType::SEDAN, &gmEngine,     &mandoBrake, &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Sedan_GM_Mando_Mobis)                { Car c(CarType::SEDAN, &gmEngine,     &mandoBrake, &mobisSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Sedan_GM_Bosch_Bosch)                { Car c(CarType::SEDAN, &gmEngine,     &boschBrake, &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Sedan_Toyota_Mando_Bosch)            { Car c(CarType::SEDAN, &toyotaEngine, &mandoBrake, &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Sedan_Toyota_Mando_Mobis)            { Car c(CarType::SEDAN, &toyotaEngine, &mandoBrake, &mobisSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Sedan_Toyota_Bosch_Bosch)            { Car c(CarType::SEDAN, &toyotaEngine, &boschBrake, &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Sedan_Wia_Mando_Bosch)               { Car c(CarType::SEDAN, &wiaEngine,    &mandoBrake, &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Sedan_Wia_Mando_Mobis)               { Car c(CarType::SEDAN, &wiaEngine,    &mandoBrake, &mobisSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Sedan_Wia_Bosch_Bosch)               { Car c(CarType::SEDAN, &wiaEngine,    &boschBrake, &boschSteering); EXPECT_TRUE(validator.isValid(c)); }

// SUV
TEST_F(CarValidatorTest, Pass_Suv_GM_Mando_Bosch)                  { Car c(CarType::SUV, &gmEngine,  &mandoBrake,       &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Suv_GM_Mando_Mobis)                  { Car c(CarType::SUV, &gmEngine,  &mandoBrake,       &mobisSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Suv_GM_Continental_Bosch)            { Car c(CarType::SUV, &gmEngine,  &continentalBrake, &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Suv_GM_Continental_Mobis)            { Car c(CarType::SUV, &gmEngine,  &continentalBrake, &mobisSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Suv_GM_Bosch_Bosch)                  { Car c(CarType::SUV, &gmEngine,  &boschBrake,       &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Suv_Wia_Mando_Bosch)                 { Car c(CarType::SUV, &wiaEngine, &mandoBrake,       &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Suv_Wia_Mando_Mobis)                 { Car c(CarType::SUV, &wiaEngine, &mandoBrake,       &mobisSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Suv_Wia_Continental_Bosch)           { Car c(CarType::SUV, &wiaEngine, &continentalBrake, &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Suv_Wia_Continental_Mobis)           { Car c(CarType::SUV, &wiaEngine, &continentalBrake, &mobisSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Suv_Wia_Bosch_Bosch)                 { Car c(CarType::SUV, &wiaEngine, &boschBrake,       &boschSteering); EXPECT_TRUE(validator.isValid(c)); }

// TRUCK
TEST_F(CarValidatorTest, Pass_Truck_GM_Continental_Bosch)          { Car c(CarType::TRUCK, &gmEngine,     &continentalBrake, &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Truck_GM_Continental_Mobis)          { Car c(CarType::TRUCK, &gmEngine,     &continentalBrake, &mobisSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Truck_GM_Bosch_Bosch)                { Car c(CarType::TRUCK, &gmEngine,     &boschBrake,       &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Truck_Toyota_Continental_Bosch)      { Car c(CarType::TRUCK, &toyotaEngine, &continentalBrake, &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Truck_Toyota_Continental_Mobis)      { Car c(CarType::TRUCK, &toyotaEngine, &continentalBrake, &mobisSteering); EXPECT_TRUE(validator.isValid(c)); }
TEST_F(CarValidatorTest, Pass_Truck_Toyota_Bosch_Bosch)            { Car c(CarType::TRUCK, &toyotaEngine, &boschBrake,       &boschSteering); EXPECT_TRUE(validator.isValid(c)); }
