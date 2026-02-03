#ifndef RATE_H
#define RATE_H

//#include "core/Vehicle.h"

struct Rate {
    double hourlyRate;     // dollars per hour
    double dailyCap;       // max charge per day
    double dayPassPrice;   // flat day pass option

    int billingIncrementMinutes = 30; // 30-minute increments (locked requirement)

    // Computes the charge for a given parked duration in minutes.
    // Rounds UP to the nearest billing increment.
    double computeCharge(int parkedMinutes, bool useDayPass) const;
};


#endif
