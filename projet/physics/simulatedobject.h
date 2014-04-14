#ifndef SIMULATEDOBJECT_H
#define SIMULATEDOBJECT_H

#include "utils.h"

//! An object simulated with bullet
/*!
        Represents an object "simulated" with bullet. The data of the object has to be saved in the _previous_state and _current_state objects every possible time in order to ensure
        that the calculations work
    */
class SimulatedObject
{
public:
    SimulatedObject();

    state_data _previous_state,_current_state;
};

#endif // SIMULATEDOBJECT_H
