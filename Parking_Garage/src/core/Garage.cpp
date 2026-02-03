#include "core/Garage.h"

Garage::Garage(int capacity)
    : capacity_(capacity) {}

Rate Garage::getRate(VehicleType type) const {
    auto it = rates_.find(type);
    if (it != rates_.end()) return it->second;

    // Fallback if not configured (shouldn't happen if you seedRates)
    return Rate{0.0, 0.0, 0.0};
}

void Garage::setRate(VehicleType type, const Rate& rate) {
    rates_[type] = rate;
}

bool Garage::isFull() const {
    return static_cast<int>(activeByPlate_.size()) >= capacity_;
}

int Garage::getCapacity() const {
    return capacity_;
}

int Garage::getOccupancy() const {
    return static_cast<int>(activeByPlate_.size());
}

int Garage::enterVehicle(const std::string& plate, VehicleType type, int entryMinutes, bool useDayPass) {
    if (isFull()) return -1;
    if (activeByPlate_.find(plate) != activeByPlate_.end()) return -1; // already inside

    int id = nextTicketId_++;
    Ticket t(id, plate, type, entryMinutes);

    activeByPlate_.emplace(plate, t);
    plateByTicketId_.emplace(id, plate);

    // store the plan choice at ENTRY
    dayPassByPlate_[plate] = useDayPass;

    return id;
}

double Garage::exitByPlate(const std::string& plate, int exitMinutes) {
    auto it = activeByPlate_.find(plate);
    if (it == activeByPlate_.end()) return -1.0;

    // look up stored plan (defaults to hourly if missing)
    bool useDayPass = false;
    auto passIt = dayPassByPlate_.find(plate);
    if (passIt != dayPassByPlate_.end()) {
        useDayPass = passIt->second;
    }

    const Ticket& t = it->second;
    double amount = computeExitCharge(t, exitMinutes, useDayPass);

    // remove from all maps
    plateByTicketId_.erase(t.getId());
    activeByPlate_.erase(it);
    dayPassByPlate_.erase(plate);

    return amount;
}

double Garage::exitByTicketId(int ticketId, int exitMinutes) {
    auto it = plateByTicketId_.find(ticketId);
    if (it == plateByTicketId_.end()) return -1.0;

    std::string plate = it->second;   // COPY (prevents dangling reference)
    return exitByPlate(plate, exitMinutes);
}

double Garage::computeExitCharge(const Ticket& t, int exitMinutes, bool useDayPass) const {
    int parkedMinutes = exitMinutes - t.getEntryMinutes();
    if (parkedMinutes < 0) parkedMinutes = 0;

    auto rateIt = rates_.find(t.getType());
    if (rateIt == rates_.end()) {
        // No rate configured: treat as $0 to avoid crashing.
        return 0.0;
    }

    return rateIt->second.computeCharge(parkedMinutes, useDayPass);
}
