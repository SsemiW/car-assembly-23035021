#include "gmock/gmock.h"
#include "../core/Assemble.h"

// =====================================================================
// C1: SEDAN + CONTINENTAL (엔진·조향 무관, 6가지)
// =====================================================================
TEST(C1, Sedan_GM_Continental_Bosch)     { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(C1, Sedan_GM_Continental_Mobis)     { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(C1, Sedan_Toyota_Continental_Bosch) { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(C1, Sedan_Toyota_Continental_Mobis) { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(C1, Sedan_Wia_Continental_Bosch)    { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(C1, Sedan_Wia_Continental_Mobis)    { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }

// =====================================================================
// C2: SUV + TOYOTA (제동·조향 무관, 6가지)
// =====================================================================
TEST(C2, Suv_Toyota_Mando_Bosch)         { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::MANDO,       SteeringSystem::BOSCH})); }
TEST(C2, Suv_Toyota_Mando_Mobis)         { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::MANDO,       SteeringSystem::MOBIS})); }
TEST(C2, Suv_Toyota_Continental_Bosch)   { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(C2, Suv_Toyota_Continental_Mobis)   { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(C2, Suv_Toyota_Bosch_Bosch)         { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }
TEST(C2, Suv_Toyota_Bosch_Mobis)         { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::BOSCH,       SteeringSystem::MOBIS})); } // C2+C5

// =====================================================================
// C3: TRUCK + WIA (제동·조향 무관, 6가지)
// =====================================================================
TEST(C3, Truck_Wia_Mando_Bosch)          { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::MANDO,       SteeringSystem::BOSCH})); } // C3+C4
TEST(C3, Truck_Wia_Mando_Mobis)          { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::MANDO,       SteeringSystem::MOBIS})); } // C3+C4
TEST(C3, Truck_Wia_Continental_Bosch)    { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(C3, Truck_Wia_Continental_Mobis)    { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(C3, Truck_Wia_Bosch_Bosch)          { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }
TEST(C3, Truck_Wia_Bosch_Mobis)          { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::BOSCH,       SteeringSystem::MOBIS})); } // C3+C5

// =====================================================================
// C4: TRUCK + MANDO (WIA 엔진 제외, 4가지)
// =====================================================================
TEST(C4, Truck_GM_Mando_Bosch)           { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::MANDO, SteeringSystem::BOSCH})); }
TEST(C4, Truck_GM_Mando_Mobis)           { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::MANDO, SteeringSystem::MOBIS})); }
TEST(C4, Truck_Toyota_Mando_Bosch)       { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::MANDO, SteeringSystem::BOSCH})); }
TEST(C4, Truck_Toyota_Mando_Mobis)       { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::MANDO, SteeringSystem::MOBIS})); }

// =====================================================================
// C5: BOSCH brake + MOBIS steering (C2/C3 중복 제외, 7가지)
// =====================================================================
TEST(C5, Sedan_GM_Bosch_Mobis)           { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Sedan_Toyota_Bosch_Mobis)       { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Sedan_Wia_Bosch_Mobis)          { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Suv_GM_Bosch_Mobis)             { EXPECT_FALSE(isValidCheck({CarType::SUV,   Engine::GM,     BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Suv_Wia_Bosch_Mobis)            { EXPECT_FALSE(isValidCheck({CarType::SUV,   Engine::WIA,    BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Truck_GM_Bosch_Mobis)           { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Truck_Toyota_Bosch_Mobis)       { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }

// =====================================================================
// PASS: 제약조건 미해당 유효 조합 (21가지)
// =====================================================================

// SEDAN (CONTINENTAL 제외)
TEST(Pass, Sedan_GM_Mando_Bosch)         { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::MANDO, SteeringSystem::BOSCH})); }
TEST(Pass, Sedan_GM_Mando_Mobis)         { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::MANDO, SteeringSystem::MOBIS})); }
TEST(Pass, Sedan_GM_Bosch_Bosch)         { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::BOSCH, SteeringSystem::BOSCH})); }
TEST(Pass, Sedan_Toyota_Mando_Bosch)     { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::MANDO, SteeringSystem::BOSCH})); }
TEST(Pass, Sedan_Toyota_Mando_Mobis)     { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::MANDO, SteeringSystem::MOBIS})); }
TEST(Pass, Sedan_Toyota_Bosch_Bosch)     { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::BOSCH, SteeringSystem::BOSCH})); }
TEST(Pass, Sedan_Wia_Mando_Bosch)        { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::MANDO, SteeringSystem::BOSCH})); }
TEST(Pass, Sedan_Wia_Mando_Mobis)        { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::MANDO, SteeringSystem::MOBIS})); }
TEST(Pass, Sedan_Wia_Bosch_Bosch)        { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::BOSCH, SteeringSystem::BOSCH})); }

// SUV (TOYOTA 엔진 제외)
TEST(Pass, Suv_GM_Mando_Bosch)           { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::GM,  BrakeSystem::MANDO,       SteeringSystem::BOSCH})); }
TEST(Pass, Suv_GM_Mando_Mobis)           { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::GM,  BrakeSystem::MANDO,       SteeringSystem::MOBIS})); }
TEST(Pass, Suv_GM_Continental_Bosch)     { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::GM,  BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(Pass, Suv_GM_Continental_Mobis)     { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::GM,  BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(Pass, Suv_GM_Bosch_Bosch)           { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::GM,  BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }
TEST(Pass, Suv_Wia_Mando_Bosch)          { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::WIA, BrakeSystem::MANDO,       SteeringSystem::BOSCH})); }
TEST(Pass, Suv_Wia_Mando_Mobis)          { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::WIA, BrakeSystem::MANDO,       SteeringSystem::MOBIS})); }
TEST(Pass, Suv_Wia_Continental_Bosch)    { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::WIA, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(Pass, Suv_Wia_Continental_Mobis)    { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::WIA, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(Pass, Suv_Wia_Bosch_Bosch)          { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::WIA, BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }

// TRUCK (WIA 엔진·MANDO 제동 제외)
TEST(Pass, Truck_GM_Continental_Bosch)   { EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(Pass, Truck_GM_Continental_Mobis)   { EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(Pass, Truck_GM_Bosch_Bosch)         { EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }
TEST(Pass, Truck_Toyota_Continental_Bosch){ EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(Pass, Truck_Toyota_Continental_Mobis){ EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(Pass, Truck_Toyota_Bosch_Bosch)     { EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }
