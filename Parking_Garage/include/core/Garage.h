#ifndef GARAGE_H
#define GARAGE_H

#include <string>
#include <unordered_map>

#include "core/Ticket.h"
#include "core/Rate.h"

class Garage {
public:
    explicit Garage(int capacity);

    // Allow main.cpp to display pricing at ENTRY
    Rate getRate(VehicleType type) const;

    void setRate(VehicleType type, const Rate& rate);

    bool isFull() const;
    int getCapacity() const;
    int getOccupancy() const;

    // Entry (automatic plate scan)
    // Returns ticketId on success, or -1 if garage full or plate already inside.
    // NEW: store plan choice at entry (day pass vs hourly)
    int enterVehicle(const std::string& plate, VehicleType type, int entryMinutes, bool useDayPass);

    // Exit (lookup by plate)
    // Returns amount charged, or -1.0 if not found.
    // UPDATED: no longer asks for plan at exit (it was chosen at entry)
    double exitByPlate(const std::string& plate, int exitMinutes);

    // Exit (lookup by ticket id)
    // Returns amount charged, or -1.0 if not found.
    // UPDATED: no longer asks for plan at exit (it was chosen at entry)
    double exitByTicketId(int ticketId, int exitMinutes);

private:
    int capacity_;
    int nextTicketId_ = 1000;

    // Active tickets
    std::unordered_map<std::string, Ticket> activeByPlate_; // plate -> ticket
    std::unordered_map<int, std::string> plateByTicketId_;  // ticketId -> plate

    // NEW: store plan choice per active plate
    std::unordered_map<std::string, bool> dayPassByPlate_;  // plate -> useDayPass

    // Rates by vehicle type
    std::unordered_map<VehicleType, Rate> rates_;

    // UPDATED: plan comes from stored dayPassByPlate_
    double computeExitCharge(const Ticket& t, int exitMinutes, bool useDayPass) const;
};

#endif
