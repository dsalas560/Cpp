#include "core/Rate.h"
#include <cmath>

double Rate::computeCharge(int parkedMinutes, bool useDayPass) const {
    if (parkedMinutes < 0) parkedMinutes = 0;

    if (useDayPass) {
        return dayPassPrice;
    }

    // Round up to nearest billing increment (30 minutes)
    int inc = billingIncrementMinutes;
    int billedMinutes = static_cast<int>(std::ceil(parkedMinutes / static_cast<double>(inc))) * inc;

    // Convert minutes -> hours
    double billedHours = billedMinutes / 60.0;

    double cost = billedHours * hourlyRate;

    // Apply daily cap
    if (cost > dailyCap) cost = dailyCap;

    return cost;
}
