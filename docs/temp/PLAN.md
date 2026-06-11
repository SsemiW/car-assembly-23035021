# CarAssembly 리팩토링 계획

## 개요

현재 `main.cpp` 하나에 모든 로직이 집중된 절차지향 코드를 단계적으로 리팩토링한다.  
**테스트 가능한 구조 확보 → 단위 테스트 작성 → 작은 변경(method 수준) → 큰 변경(class 수준)** 순서로 진행하여 각 단계마다 동작을 보장한다.

> **규칙: STEP 3부터는 모든 커밋 전에 STEP 2에서 작성한 단위 테스트 전체(54개)가 통과해야 한다.**  
> 테스트가 하나라도 실패하면 해당 변경을 커밋하지 않고 원인을 먼저 수정한다.

## 단계 요약

| 단계 | 범위 | 핵심 변경 | 테스트 조건 | 위험도 |
|------|------|-----------|-------------|--------|
| STEP 1 | 테스트 가능한 구조 | enum class·CarConfig 도입, isValidCheck() 인자화, 파일 분리 | 없음 | 낮음 |
| STEP 2 | 단위 테스트 작성 | isValidCheck() 54개 케이스 작성 | 없음 | 없음 |
| STEP 3 | 메서드 | delay 교체·네이밍 통일·입력 파싱 개선 | **커밋마다 전체 통과** | 낮음 |
| STEP 4 | 함수 | printMenu / validateInput / printCarInfo 추출, 중복 제거 | **커밋마다 전체 통과** | 낮음 |
| STEP 5 | 클래스 | 부품 계층·Car·CarValidator·CarAssembler 분리 | **커밋마다 전체 통과** | 높음 |

---

## STEP 1 — 테스트 가능한 구조로 전환

> 단위 테스트를 작성하기 위한 최소 구조 변경이다. 비즈니스 로직은 건드리지 않는다.  
> 현재 `isValidCheck()`는 전역 `stack[]`을 직접 읽으므로 테스트 코드에서 독립적으로 호출할 수 없다.

### 1-1. `enum` → `enum class` 로 교체

`CarConfig` 구조체의 필드 타입으로 사용하기 위해 먼저 전환한다.

```cpp
// Before
enum CarType { SEDAN = 1, SUV, TRUCK };
enum Engine  { GM = 1, TOYOTA, WIA };
enum brakeSystem { MANDO = 1, CONTINENTAL, BOSCH_B };
enum SteeringSystem { BOSCH_S = 1, MOBIS };

// After
enum class CarType       { SEDAN = 1, SUV, TRUCK };
enum class Engine        { GM = 1, TOYOTA, WIA };
enum class BrakeSystem   { MANDO = 1, CONTINENTAL, BOSCH };
enum class SteeringSystem{ BOSCH = 1, MOBIS };
```

- `BOSCH_B` / `BOSCH_S` 같은 접미사 없이도 타입 안전하게 구분 가능
- 전역 네임스페이스 오염 방지

### 1-2. `isValidCheck()` 반환 타입 수정 및 불필요한 코드 제거

```cpp
// Before
int isValidCheck() { ... return false; ... return true; }
return true; // 도달 불가능

// After
bool isValidCheck() { ... return false; ... return true; }
// 마지막 return true; 제거
```

### 1-3. `CarConfig` 구조체 도입 및 전역 상태 제거

전역 `int stack[10]` 을 의미 있는 구조체로 교체하여 테스트에서 직접 값을 구성할 수 있게 한다.

```cpp
struct CarConfig {
    CarType        carType;
    Engine         engine;
    BrakeSystem    brakeSystem;
    SteeringSystem steeringSystem;
};
```

- `stack[CarType_Q]` 같은 매직 인덱스 접근을 의미 있는 필드명으로 대체
- `main()` 안에서 로컬 변수로 선언하여 전역 상태 제거

### 1-4. `isValidCheck()` 시그니처 변경

전역 `stack[]` 의존을 끊고 `CarConfig`를 인자로 받도록 변경한다.  
`runProducedCar()` / `testProducedCar()` 도 동일하게 변경한다.

```cpp
// Before: 전역 stack[] 직접 참조
bool isValidCheck() { if (stack[CarType_Q] == SEDAN ...) }

// After: 인자로 전달받아 참조
bool isValidCheck(const CarConfig& config);
void runProducedCar(const CarConfig& config);
void testProducedCar(const CarConfig& config);
```

### 1-5. 비즈니스 로직 파일 분리

테스트 코드(`#ifdef _DEBUG`)에서 `isValidCheck()`를 호출하려면 해당 함수가 공유 가능한 파일에 있어야 한다.

```
// Before: 모든 코드가 main.cpp 의 #else 블록 안에 존재
main.cpp  (#ifdef _DEBUG ... #else ... #endif)

// After
main.cpp          ← 진입점 및 UI 루프만 유지
assemble.h        ← CarConfig, enum class, isValidCheck() 선언
assemble.cpp      ← isValidCheck(), runProducedCar(), testProducedCar() 구현
```

- Debug 빌드의 테스트 코드와 Release 빌드의 main 코드가 동일한 `assemble.h`를 include

---

## STEP 2 — 단위 테스트 작성

> STEP 1로 확보한 구조를 바탕으로 현재 동작을 테스트로 고정한다.  
> 이후 모든 리팩토링의 안전망 역할을 한다.

테스트 대상 조합 전체 수: CarType(3) × Engine(3) × BrakeSystem(3) × SteeringSystem(2) = **54가지**

5가지 제약조건을 기준으로 FAIL / PASS 를 분류하여 전부 커버한다.

---

### 제약조건 정리

| # | 조건 | 설명 |
|---|------|------|
| C1 | SEDAN + CONTINENTAL | Continental은 Sedan용 제동장치를 만들지 않는다 |
| C2 | SUV + TOYOTA | TOYOTA는 SUV용 엔진을 만들지 않는다 |
| C3 | TRUCK + WIA | WIA는 Truck용 엔진을 만들지 않는다 |
| C4 | TRUCK + MANDO | MANDO는 Truck용 제동장치를 만들지 않는다 |
| C5 | BOSCH brake + MOBIS steering | Bosch 제동장치는 타사 조향장치와 호환되지 않는다 |

---

### 2-1. FAIL 케이스 — C1: SEDAN + CONTINENTAL (엔진·조향 무관, 6가지)

```cpp
// SEDAN + CONTINENTAL + GM + BOSCH
TEST(C1, Sedan_GM_Continental_Bosch)    { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
// SEDAN + CONTINENTAL + GM + MOBIS
TEST(C1, Sedan_GM_Continental_Mobis)    { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
// SEDAN + CONTINENTAL + TOYOTA + BOSCH
TEST(C1, Sedan_Toyota_Continental_Bosch){ EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
// SEDAN + CONTINENTAL + TOYOTA + MOBIS
TEST(C1, Sedan_Toyota_Continental_Mobis){ EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
// SEDAN + CONTINENTAL + WIA + BOSCH
TEST(C1, Sedan_Wia_Continental_Bosch)   { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
// SEDAN + CONTINENTAL + WIA + MOBIS
TEST(C1, Sedan_Wia_Continental_Mobis)   { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
```

### 2-2. FAIL 케이스 — C2: SUV + TOYOTA (제동·조향 무관, 6가지)

```cpp
TEST(C2, Suv_Toyota_Mando_Bosch)        { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::MANDO,       SteeringSystem::BOSCH})); }
TEST(C2, Suv_Toyota_Mando_Mobis)        { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::MANDO,       SteeringSystem::MOBIS})); }
TEST(C2, Suv_Toyota_Continental_Bosch)  { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(C2, Suv_Toyota_Continental_Mobis)  { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(C2, Suv_Toyota_Bosch_Bosch)        { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }
TEST(C2, Suv_Toyota_Bosch_Mobis)        { EXPECT_FALSE(isValidCheck({CarType::SUV, Engine::TOYOTA, BrakeSystem::BOSCH,       SteeringSystem::MOBIS})); } // C2+C5
```

### 2-3. FAIL 케이스 — C3: TRUCK + WIA (제동·조향 무관, 6가지)

```cpp
TEST(C3, Truck_Wia_Mando_Bosch)         { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::MANDO,       SteeringSystem::BOSCH})); } // C3+C4
TEST(C3, Truck_Wia_Mando_Mobis)         { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::MANDO,       SteeringSystem::MOBIS})); } // C3+C4
TEST(C3, Truck_Wia_Continental_Bosch)   { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(C3, Truck_Wia_Continental_Mobis)   { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(C3, Truck_Wia_Bosch_Bosch)         { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }
TEST(C3, Truck_Wia_Bosch_Mobis)         { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::WIA, BrakeSystem::BOSCH,       SteeringSystem::MOBIS})); } // C3+C5
```

### 2-4. FAIL 케이스 — C4: TRUCK + MANDO (WIA 엔진 제외, 4가지)

```cpp
// WIA 엔진 조합은 C3에서 이미 커버
TEST(C4, Truck_GM_Mando_Bosch)          { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::MANDO, SteeringSystem::BOSCH})); }
TEST(C4, Truck_GM_Mando_Mobis)          { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::MANDO, SteeringSystem::MOBIS})); }
TEST(C4, Truck_Toyota_Mando_Bosch)      { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::MANDO, SteeringSystem::BOSCH})); }
TEST(C4, Truck_Toyota_Mando_Mobis)      { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::MANDO, SteeringSystem::MOBIS})); }
```

### 2-5. FAIL 케이스 — C5: BOSCH brake + MOBIS steering (중복 미포함, 7가지)

```cpp
// SUV+TOYOTA, TRUCK+WIA 조합은 C2/C3에서 이미 커버
TEST(C5, Sedan_GM_Bosch_Mobis)          { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Sedan_Toyota_Bosch_Mobis)      { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Sedan_Wia_Bosch_Mobis)         { EXPECT_FALSE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Suv_GM_Bosch_Mobis)            { EXPECT_FALSE(isValidCheck({CarType::SUV,   Engine::GM,     BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Suv_Wia_Bosch_Mobis)           { EXPECT_FALSE(isValidCheck({CarType::SUV,   Engine::WIA,    BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Truck_GM_Bosch_Mobis)          { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
TEST(C5, Truck_Toyota_Bosch_Mobis)      { EXPECT_FALSE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::BOSCH, SteeringSystem::MOBIS})); }
```

### 2-6. PASS 케이스 (제약조건에 해당하지 않는 유효 조합, 25가지)

```cpp
// --- SEDAN (CONTINENTAL 제외) ---
TEST(Pass, Sedan_GM_Mando_Bosch)        { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::MANDO, SteeringSystem::BOSCH})); }
TEST(Pass, Sedan_GM_Mando_Mobis)        { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::MANDO, SteeringSystem::MOBIS})); }
TEST(Pass, Sedan_GM_Bosch_Bosch)        { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::GM,     BrakeSystem::BOSCH, SteeringSystem::BOSCH})); }
TEST(Pass, Sedan_Toyota_Mando_Bosch)    { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::MANDO, SteeringSystem::BOSCH})); }
TEST(Pass, Sedan_Toyota_Mando_Mobis)    { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::MANDO, SteeringSystem::MOBIS})); }
TEST(Pass, Sedan_Toyota_Bosch_Bosch)    { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::TOYOTA, BrakeSystem::BOSCH, SteeringSystem::BOSCH})); }
TEST(Pass, Sedan_Wia_Mando_Bosch)       { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::MANDO, SteeringSystem::BOSCH})); }
TEST(Pass, Sedan_Wia_Mando_Mobis)       { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::MANDO, SteeringSystem::MOBIS})); }
TEST(Pass, Sedan_Wia_Bosch_Bosch)       { EXPECT_TRUE(isValidCheck({CarType::SEDAN, Engine::WIA,    BrakeSystem::BOSCH, SteeringSystem::BOSCH})); }

// --- SUV (TOYOTA 엔진 제외) ---
TEST(Pass, Suv_GM_Mando_Bosch)          { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::GM,  BrakeSystem::MANDO,       SteeringSystem::BOSCH})); }
TEST(Pass, Suv_GM_Mando_Mobis)          { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::GM,  BrakeSystem::MANDO,       SteeringSystem::MOBIS})); }
TEST(Pass, Suv_GM_Continental_Bosch)    { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::GM,  BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(Pass, Suv_GM_Continental_Mobis)    { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::GM,  BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(Pass, Suv_GM_Bosch_Bosch)          { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::GM,  BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }
TEST(Pass, Suv_Wia_Mando_Bosch)         { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::WIA, BrakeSystem::MANDO,       SteeringSystem::BOSCH})); }
TEST(Pass, Suv_Wia_Mando_Mobis)         { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::WIA, BrakeSystem::MANDO,       SteeringSystem::MOBIS})); }
TEST(Pass, Suv_Wia_Continental_Bosch)   { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::WIA, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(Pass, Suv_Wia_Continental_Mobis)   { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::WIA, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(Pass, Suv_Wia_Bosch_Bosch)         { EXPECT_TRUE(isValidCheck({CarType::SUV, Engine::WIA, BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }

// --- TRUCK (WIA 엔진·MANDO 제동 제외) ---
TEST(Pass, Truck_GM_Continental_Bosch)  { EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(Pass, Truck_GM_Continental_Mobis)  { EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(Pass, Truck_GM_Bosch_Bosch)        { EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::GM,     BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }
TEST(Pass, Truck_Toyota_Continental_Bosch){ EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::BOSCH})); }
TEST(Pass, Truck_Toyota_Continental_Mobis){ EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS})); }
TEST(Pass, Truck_Toyota_Bosch_Bosch)    { EXPECT_TRUE(isValidCheck({CarType::TRUCK, Engine::TOYOTA, BrakeSystem::BOSCH,       SteeringSystem::BOSCH})); }
```

### 케이스 집계

| 분류 | 케이스 수 |
|------|-----------|
| FAIL — C1 (SEDAN + CONTINENTAL) | 6 |
| FAIL — C2 (SUV + TOYOTA) | 6 |
| FAIL — C3 (TRUCK + WIA) | 6 |
| FAIL — C4 (TRUCK + MANDO, C3 중복 제외) | 4 |
| FAIL — C5 (BOSCH brake + MOBIS, C2/C3 중복 제외) | 7 |
| PASS | 25 |
| **합계** | **54** |

> C2+C5, C3+C4, C3+C5 중복 조합 4가지는 먼저 매칭되는 제약조건 테스트에서만 작성하여  
> 전체 54가지 조합을 빠짐없이 커버한다.

---

## STEP 3 — 메서드 수준 코드 정리

> **단위 테스트 전체 통과 필수.** 각 항목을 완료할 때마다 Debug 빌드로 테스트를 실행하고 전부 통과한 상태에서만 커밋한다.  
> 동작 변경 없이 코드 품질만 개선한다.

### 3-1. `delay()` 구현 교체

```cpp
// Before: CPU를 낭비하는 busy-wait
void delay(int ms) {
    volatile int sum = 0;
    for (int i = 0; i < 1000; i++) { ... }
}

// After
#include <thread>
#include <chrono>
void delay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
```

### 3-2. 함수 이름 통일 (네이밍 컨벤션)

```cpp
// Before: 소문자 시작으로 불일치
void selectbrakeSystem(int answer);

// After: 모든 select 함수를 camelCase로 통일
void selectBrakeSystem(int answer);
```

### 3-3. 입력 파싱 방식 개선

```cpp
// Before: raw char*, strtok_s를 두 번 호출
char* context = nullptr;
strtok_s(buf, "\r", &context);
strtok_s(buf, "\n", &context);

// After: std::string으로 교체하여 개행 일괄 제거
std::string input(buf);
input.erase(input.find_last_not_of("\r\n") + 1);
```

---

## STEP 4 — 함수 수준 책임 분리

> **단위 테스트 전체 통과 필수.** 각 항목을 완료할 때마다 Debug 빌드로 테스트를 실행하고 전부 통과한 상태에서만 커밋한다.  
> `main()` 안에 뒤섞인 역할을 목적별 함수로 추출한다.

### 4-1. `printMenu()` 추출

`main()` 루프 안의 각 단계별 출력 블록을 별도 함수로 분리한다.

```cpp
void printMenu(int step);
```

- `main()`에서 화면 출력 코드를 제거하고 `printMenu(step)` 한 줄로 대체
- 이후 클래스 전환 시 UI 담당 클래스로 이동하기 용이

### 4-2. `validateInput()` 추출

각 step별로 흩어진 범위 검증 `if` 블록들을 하나의 함수로 통합한다.

```cpp
bool validateInput(int step, int answer);
```

### 4-3. `printCarInfo()` 추출 (`runProducedCar()` 분리)

```cpp
void printCarInfo(const CarConfig& config);  // 부품 정보 출력
void runProducedCar(const CarConfig& config); // 유효성 확인 후 printCarInfo() 호출
```

### 4-4. `testProducedCar()` — `isValidCheck()` 중복 제거

`testProducedCar()`와 `runProducedCar()` 모두 동일한 조합 검증 로직을 직접 가지고 있다.

```cpp
// After: testProducedCar()는 isValidCheck() 결과만 출력
void testProducedCar(const CarConfig& config) {
    if (!isValidCheck(config))
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
    else
        printf("자동차 부품 조합 테스트 결과 : PASS\n");
}
```

---

## STEP 5 — 클래스 수준 분리

> **단위 테스트 전체 통과 필수.** 각 항목을 완료할 때마다 Debug 빌드로 테스트를 실행하고 전부 통과한 상태에서만 커밋한다.  
> 부품과 차량을 객체로 모델링하고, 관심사를 클래스 단위로 분리한다.

### 5-1. 부품 클래스 계층 구조

각 부품을 추상 기반 클래스 + 구체 클래스로 분리한다.

```
IPart (interface)
├── IEngine
│   ├── GmEngine
│   ├── ToyotaEngine
│   └── WiaEngine
├── IBrakeSystem
│   ├── MandoBrake
│   ├── ContinentalBrake
│   └── BoschBrake
└── ISteeringSystem
    ├── BoschSteering
    └── MobisSteering
```

### 5-2. `Car` 클래스

선택된 부품을 보유하고 자신을 출력하는 책임을 갖는다.

```cpp
class Car {
public:
    Car(CarType type, IEngine*, IBrakeSystem*, ISteeringSystem*);
    void printInfo() const;
    CarType getType() const;
    IEngine* getEngine() const;
    IBrakeSystem* getBrakeSystem() const;
    ISteeringSystem* getSteeringSystem() const;
};
```

### 5-3. `CarValidator` 클래스

조합 유효성 검증 로직을 담당한다.  
`isValidCheck()` 함수가 이 클래스의 메서드로 이동한다.

```cpp
class CarValidator {
public:
    bool isValid(const Car& car) const;
    std::string getFailReason(const Car& car) const;
};
```

- gmock으로 `CarValidator` 자체를 모킹하여 상위 로직 테스트 가능

### 5-4. `CarAssembler` 클래스

사용자 입력을 받아 `Car` 객체를 생성하는 UI 흐름을 담당한다.  
현재 `main()` 루프 전체가 이 클래스의 `run()` 메서드로 이동한다.

```cpp
class CarAssembler {
public:
    void run();
private:
    void printMenu(int step) const;
    bool validateInput(int step, int answer) const;
    Car buildCar() const;
};
```

### 5-5. `main()` 정리

리팩토링 완료 후 `main()`은 진입점 역할만 수행한다.

```cpp
int main() {
    CarAssembler assembler;
    assembler.run();
    return 0;
}
```

---

## 파일 구조 (목표)

```
CarAssembly/
├── main.cpp
├── assemble.h / assemble.cpp       ← STEP 1 에서 분리 (테스트 공유)
├── Car.h / Car.cpp
├── CarValidator.h / CarValidator.cpp
├── CarAssembler.h / CarAssembler.cpp
├── parts/
│   ├── IEngine.h
│   ├── IBrakeSystem.h
│   ├── ISteeringSystem.h
│   ├── Engine.h / Engine.cpp
│   ├── BrakeSystem.h / BrakeSystem.cpp
│   └── SteeringSystem.h / SteeringSystem.cpp
└── tests/
    ├── assemble_test.cpp            ← STEP 2 에서 작성
    ├── CarValidatorTest.cpp
    └── CarAssemblerTest.cpp
```

