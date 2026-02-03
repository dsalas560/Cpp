#include "core/Vehicle.h"

Vehicle::Vehicle(const std::string& licensePlate, VehicleType type)
    : licensePlate_(licensePlate), type_(type) {}

std::string Vehicle::getLicensePlate() const {
    return licensePlate_;
}

VehicleType Vehicle::getType() const {
    return type_;
}
