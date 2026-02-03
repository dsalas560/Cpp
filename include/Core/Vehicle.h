#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

enum class VehicleType {
    Car,
    Motorcycle,
    Truck
};

class Vehicle {
public:
    Vehicle(const std::string& licensePlate, VehicleType type);

    std::string getLicensePlate() const;
    VehicleType getType() const;

private:
    std::string licensePlate_;
    VehicleType type_;
};

#endif
