#ifndef TICKET_H
#define TICKET_H

#include <string>
#include "core/Vehicle.h"

class Ticket {
public:
    Ticket(int ticketId, const std::string& licensePlate, VehicleType type, int entryMinutes);

    int getId() const;
    std::string getLicensePlate() const;
    VehicleType getType() const;
    int getEntryMinutes() const;

private:
    int ticketId_;
    std::string licensePlate_;
    VehicleType type_;
    int entryMinutes_;
};

#endif
