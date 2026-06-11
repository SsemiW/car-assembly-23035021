# CarAssembly

자동차 부품을 선택하고 조합 유효성을 검증하는 C++ 콘솔 애플리케이션

## 프로젝트 개요

사용자가 차량 타입, 엔진, 제동장치, 조향장치를 단계별로 선택하면 해당 조합이 실제로 제작 가능한 차량인지 검증

## 차량 제조 순서

### 1단계 - 차량 타입 선택 (추가 가능)

| 번호 | 타입  |
|------|-------|
| 1    | Sedan |
| 2    | SUV   |
| 3    | Truck |

### 2단계 - 부품 조립

| 부품          | 선택지                        |
|---------------|-------------------------------|
| Engine        | GM / TOYOTA / WIA             |
| Brake System  | MANDO / Continental / Bosch   |
| SteeringSystem| Bosch / Mobis                 |

### 3단계 - 완성 가능 조합 확인

아래 제한조건을 모두 통과해야 차량이 동작함

| 제한조건 | 내용 |
|----------|------|
| 제한 1   | Bosch 제동장치 사용 시, 조향장치도 반드시 Bosch를 사용해야 한다 |
| 제한 2   | Continental은 Sedan용 제동장치를 제조하지 않는다 |
| 제한 3   | TOYOTA는 SUV용 엔진을 제조하지 않는다 |
| 제한 4   | WIA는 Truck용 엔진을 제조하지 않는다 |
| 제한 5   | MANDO는 Truck용 제동장치를 제조하지 않는다 |


## 빌드 및 실행

**요구 환경:** Visual Studio 2022, C++17 이상

```
CarAssembly.slnx 를 Visual Studio에서 열고 빌드
```

- **Release 모드:** 차량 조립 콘솔 UI 실행
- **Debug 모드:** GoogleMock 기반 단위 테스트 실행


## 레거시 코드의 문제점 (리팩토링 대상)

현재 소스코드(`main.cpp`)는 절차지향식으로 구현되어 있으며 아래의 문제점을 가집니다.

| 문제 | 설명 |
|------|------|
| 유지보수 어려움 | 단일 파일에 모든 로직이 집중된 절차지향 구조 |
| 안전하지 않은 문법 | 전역 배열(`stack[10]`)로 상태 관리, unsafe 포인터 사용 |
| 확장성 부족 | 차량 타입·부품 추가 시 전체 코드를 수정해야 함 |
| 단위 테스트 없음 | 개별 부품·조합 로직에 대한 자동화 테스트 부재 |

## 리팩토링 목표

- **절차지향 → 객체지향:** 차량 타입, 엔진, 제동장치, 조향장치를 각각 클래스로 분리
- **안전한 코드:** 전역 변수 제거, 타입 안전성 강화
- **확장 가능한 구조:** 새로운 부품·타입 추가가 기존 코드 수정 없이 가능하도록 설계
- **단위 테스트 도입:** GoogleMock(gmock)을 활용한 부품 조합 유효성 테스트 작성


## 참고

- 레거시 소스코드: https://github.com/mincoding-ai/assemblyCar
- 관련 자료: `docs/[CRA_AI] Day2_1_Agentic Engineering.pdf` (p.20~24)


---

## 최종 파일 구조

```
CarAssembly/
├── CarAssembly.vcxproj
└── src/
    ├── main.cpp
    ├── core/
    │   ├── Assemble.h / Assemble.cpp      # enum class, CarConfig, isValidCheck()
    │   ├── Car.h / Car.cpp                # 선택된 부품 보유 및 출력
    │   ├── CarAssembler.h / CarAssembler.cpp  # UI 루프 및 Car 조립
    │   └── CarValidator.h / CarValidator.cpp  # 조합 유효성 검증
    ├── parts/
    │   ├── IEngine.h / IBrakeSystem.h / ISteeringSystem.h  # 부품 인터페이스
    │   └── Engine.h / BrakeSystem.h / SteeringSystem.h     # 구체 부품 클래스
    └── tests/
        ├── AssembleTest.cpp               # isValidCheck() 단위 테스트
        ├── CarValidatorTest.cpp           # CarValidator::isValid() 단위 테스트
        └── CarAssemblerTest.cpp           # validateInput() · buildCar() 단위 테스트
```


## 단위 테스트 파일 비교

| | `AssembleTest.cpp` | `CarValidatorTest.cpp` | `CarAssemblerTest.cpp` |
|---|---|---|---|
| 테스트 대상 | `isValidCheck(CarConfig&)` | `CarValidator::isValid(Car&)` | `validateInput()` · `buildCar()` |
| 입력 방식 | `CarConfig` 구조체에 enum 값 직접 할당 | `Car` 객체에 부품 포인터 전달 | step·answer 정수값 / `CarConfig` 구조체 |
| 검증 계층 | 비즈니스 로직 함수 단독 | 클래스 + 부품 인터페이스 체인 전체 | UI 입력 검증 + 부품 매핑 |
| 추가로 검증하는 것 | 조합 유효성 판단 로직 자체 | 각 구체 부품 클래스의 타입 반환값, `CarValidator`의 변환 경로 | 각 step별 유효 범위, `CarConfig` → `Car` 부품 포인터 매핑 |
| 케이스 수 | 54개 | 54개 | 22개 |
