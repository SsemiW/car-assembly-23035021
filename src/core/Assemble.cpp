#include "Assemble.h"
#include <functional>
#include <vector>

using ValidationRule = std::function<bool(const CarConfig&)>;

static const std::vector<ValidationRule> VALIDATION_RULES = {
    [](const CarConfig& c) { return !(c.carType == CarType::SEDAN  && c.brakeSystem == BrakeSystem::CONTINENTAL); },
    [](const CarConfig& c) { return !(c.carType == CarType::SUV    && c.engine == Engine::TOYOTA); },
    [](const CarConfig& c) { return !(c.carType == CarType::TRUCK  && c.engine == Engine::WIA); },
    [](const CarConfig& c) { return !(c.carType == CarType::TRUCK  && c.brakeSystem == BrakeSystem::MANDO); },
    [](const CarConfig& c) { return !(c.brakeSystem == BrakeSystem::BOSCH && c.steeringSystem != SteeringSystem::BOSCH); },
};

bool isValidCheck(const CarConfig& config)
{
    for (const auto& rule : VALIDATION_RULES)
        if (!rule(config)) return false;
    return true;
}
