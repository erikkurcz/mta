// Structure used to hold all current trip info 

#include <iostream>

#include "trip_map.hh"


bool TripMap::add_trip(TripInfo ti)
{
    if (k_trip_map.count(ti.trip_id) == 0)
    {
        std::cout << "Adding trip info for trip_id: " << ti.trip_id << std::endl;
        // Doesn't exist, so just add it
        k_trip_map[ti.trip_id] = ti;
        return true;
    }
    else 
    {
        // Does exist
        // Let's update the existing if it is a more recent update
        TripInfo& existing = k_trip_map[ti.trip_id];
        if (ti.pi.timestamp > existing.pi.timestamp)
        {
            // We've come across a more recent update for this trip
            // Replace what exists in k_trip_map
            std::cout << "Found more recent update than existing:\n"
                      << "\t\tNEW: " << ti 
                      << "\n\t\tvs\n"
                      << "\t\tOLD: " << existing << std::endl;
            k_trip_map[ti.trip_id] = ti;
            return true;
        }
        else
        {
            // Just came across an older update than what exists, so no action needed
            std::cout << "Found old update, not replacing existing info\n"
                      << "\t\tEXISTING: " << existing 
                      << "\n\t\tvs\n"
                      << "\t\tOLD: " << ti << std::endl;
            return false;
        }
    }
}

void TripMap::add_trips(std::vector<TripInfo>* trips)
{
    for (int i = 0; i < trips->size(); i++){
        add_trip(trips->at(i));
    }
}
