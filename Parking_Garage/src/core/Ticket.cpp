#include "core/Ticket.h"

Ticket::Ticket(int ticketId, const std::string& licensePlate, VehicleType type, int entryMinutes)
    : ticketId_(ticketId),
      licensePlate_(licensePlate),
      type_(type),
      entryMinutes_(entryMinutes) {}

int Ticket::getId() const {
    return ticketId_;
}

std::string Ticket::getLicensePlate() const {
    return licensePlate_;
}

VehicleType Ticket::getType() const {
    return type_;
}

int Ticket::getEntryMinutes() const {
    return entryMinutes_;
}
