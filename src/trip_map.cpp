// Structure used to hold all current trip info 

#include <iostream>

#include "trip_map.hh"


bool TripMap::add_trip(TripInfo new_ti)
{
    if (k_trip_map.count(new_ti.trip_id) == 0)
    {
        // Doesn't exist, so just add it
        std::cout << "Adding trip info for new trip_id: " << new_ti.trip_id << std::endl;
        TripInfoVec tiv;
        tiv.push_back(new_ti);
        k_trip_map[new_ti.trip_id] = tiv;
        return true;
    }
    else 
    {
        // Does exist - that is, a TripInfoVec for this TripId exists
        // We are therefore receiving a second update for an existing trip,
        // and will store it in chronological order relative to others
        // This will be rare when parsing a single file
        // But if using multiple files, will see many subsequent updates
        // ie daemon usage

        // Let's insert the record into the correct spot in the vector
        TripInfoVec& tiv_existing = k_trip_map[new_ti.trip_id];

        // Find correct spot
        // Guaranteed at least 1 item here
        TripInfoVec::iterator itr = tiv_existing.begin();
        while (itr != tiv_existing.end())
        {
            TripInfo tmp = *itr;
            if (tmp.pi.timestamp > new_ti.pi.timestamp)
            {
                itr++;
            }
            else
            {
                // Check if duplicate
                if (new_ti == tmp){
                    std::cout << "Duplicate trip, discarding" << std::endl;
                    itr++;
                }
                else
                {
                    std::cout << "Found insert point:\n\t" << tmp << "\n\tfor new_ti:\n\t" << new_ti << std::endl;
                    // Will have not itered if it is newest update
                    tiv_existing.insert(itr, new_ti);
                    break;
                }

            }
        }


        return true;
    }
}

void TripMap::add_trips(std::vector<TripInfo>* trips)
{
    // Considering each trip as an independent item here
    // Even though we do maintain vectors of TripInfo objects 
    //   for a single trip in the underlying structure
    for (int i = 0; i < trips->size(); i++){
        add_trip(trips->at(i));
    }
}
