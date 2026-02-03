#include <iostream>
#include <string>
#include <limits>

#include "core/Garage.h"
#include "core/Vehicle.h"
#include "core/Rate.h"

static void clearBadInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static bool readInt(const char* prompt, int& out) {
    std::cout << prompt;
    if (!(std::cin >> out)) {
        clearBadInput();
        return false;
    }
    return true;
}

// Keeps asking until user enters a valid integer in [minVal, maxVal]
static int readIntInRange(const char* prompt, int minVal, int maxVal) {
    int v;
    while (true) {
        if (!readInt(prompt, v)) {
            std::cout << "Invalid input. Enter a number.\n";
            continue;
        }
        if (v < minVal || v > maxVal) {
            std::cout << "Please enter a number from " << minVal << " to " << maxVal << ".\n";
            continue;
        }
        return v;
    }
}

static VehicleType readVehicleType() {
    int choice = readIntInRange("Select vehicle type [1=Car, 2=Motorcycle, 3=Truck]: ", 1, 3);
    if (choice == 2) return VehicleType::Motorcycle;
    if (choice == 3) return VehicleType::Truck;
    return VehicleType::Car;
}

static void seedRates(Garage& g) {
    Rate car{5.0, 25.0, 20.0};
    Rate moto{3.0, 15.0, 12.0};
    Rate truck{8.0, 40.0, 35.0};

    g.setRate(VehicleType::Car, car);
    g.setRate(VehicleType::Motorcycle, moto);
    g.setRate(VehicleType::Truck, truck);
}

static bool choosePlanAtEntry(const Garage& g, VehicleType type) {
    Rate r = g.getRate(type);

    std::cout << "\n--- Pricing ---\n";
    std::cout << "1) Hourly: $" << r.hourlyRate << " per hour (daily cap $" << r.dailyCap << ")\n";
    std::cout << "2) Day pass: $" << r.dayPassPrice << " flat\n";

    int plan = readIntInRange("Choose your rate plan (1 or 2): ", 1, 2);
    return (plan == 2);
}

int main() {
    Garage garage(50);
    seedRates(garage);

    int nowMinutes = 0; // simulated time

    while (true) {
        std::cout << "\n--- Parking Garage --- (time = " << nowMinutes << " min)\n";
        std::cout << "1) Enter vehicle\n";
        std::cout << "2) Exit by license plate\n";
        std::cout << "3) Exit by ticket ID\n";
        std::cout << "4) Status\n";
        std::cout << "5) Advance time\n";
        std::cout << "0) Quit\n";

        int cmd = readIntInRange("Select an option (0-5): ", 0, 5);
        if (cmd == 0) break;

        if (cmd == 1) {
            std::string plate;
            std::cout << "License plate: ";
            std::cin >> plate;

            VehicleType type = readVehicleType();
            bool useDayPass = choosePlanAtEntry(garage, type);

            int ticketId = garage.enterVehicle(plate, type, nowMinutes, useDayPass);
            if (ticketId == -1) {
                std::cout << "Entry failed (garage full or plate already inside).\n";
            } else {
                std::cout << "Entry OK. Ticket ID = " << ticketId
                          << " | Plan: " << (useDayPass ? "Day pass" : "Hourly") << "\n";
            }
        }
        else if (cmd == 2) {
            std::string plate;
            std::cout << "License plate: ";
            std::cin >> plate;

            double amount = garage.exitByPlate(plate, nowMinutes);
            if (amount < 0) {
                std::cout << "Exit failed (plate not found).\n";
            } else {
                std::cout << "Charge = $" << amount << "\n";
            }
        }
        else if (cmd == 3) {
            int id = readIntInRange("Ticket ID: ", 0, 99999999); // allow your 1000+ ids
            double amount = garage.exitByTicketId(id, nowMinutes);

            if (amount < 0) {
                std::cout << "Exit failed (ticket not found).\n";
            } else {
                std::cout << "Charge = $" << amount << "\n";
            }
        }
        else if (cmd == 4) {
            std::cout << "Capacity: " << garage.getCapacity()
                      << " | Occupancy: " << garage.getOccupancy()
                      << " | Available: " << (garage.getCapacity() - garage.getOccupancy())
                      << "\n";
        }
        else if (cmd == 5) {
            int minutes = readIntInRange("Advance minutes by (0-100000): ", 0, 100000);
            nowMinutes += minutes;
        }
    }

    std::cout << "Goodbye.\n";
    return 0;
}
