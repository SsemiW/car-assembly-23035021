#pragma once
#include "assemble.h"
#include "Car.h"
#include "CarValidator.h"
#include "parts/Engine.h"
#include "parts/BrakeSystem.h"
#include "parts/SteeringSystem.h"

class CarAssembler
{
public:
    void run();

private:
    enum Step { CarType_Q, Engine_Q, BrakeSystem_Q, SteeringSystem_Q, Run_Test };

    void printMenu(int step) const;
    bool validateInput(int step, int answer) const;
    Car  buildCar(const CarConfig& config);

    GmEngine         m_gmEngine;
    ToyotaEngine     m_toyotaEngine;
    WiaEngine        m_wiaEngine;
    BrokenEngine     m_brokenEngine;
    MandoBrake       m_mandoBrake;
    ContinentalBrake m_continentalBrake;
    BoschBrake       m_boschBrake;
    BoschSteering    m_boschSteering;
    MobisSteering    m_mobisSteering;
};
