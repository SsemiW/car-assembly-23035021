#include "CarValidator.h"
#include "assemble.h"

bool CarValidator::isValid(const Car& car) const
{
    CarConfig config;
    config.carType        = car.getType();
    config.engine         = car.getEngine()->engineType();
    config.brakeSystem    = car.getBrakeSystem()->brakeSystemType();
    config.steeringSystem = car.getSteeringSystem()->steeringSystemType();
    return isValidCheck(config);
}
