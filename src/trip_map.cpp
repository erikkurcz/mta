// Structure used to hold all current trip info 

#include <iostream> 
#include "trip_map.hh"

log4cxx::LoggerPtr tripmap_logger(log4cxx::Logger::getLogger("mta.TripMap"));

bool TripMap::add_trip(TripInfo new_ti)
{
    if (k_trip_map.count(new_ti.trip_id) == 0)
    {
        // Doesn't exist, so just add it
        LOG4CXX_DEBUG(tripmap_logger, "add_trip(): Adding trip info for new trip_id: " << new_ti.trip_id);
        TripInfoVec tiv;
        tiv.push_back(new_ti);
        k_trip_map[new_ti.trip_id] = tiv;
        return true;
    }
    else 
    {
        // Does exist - that is, a TripInfoVec for this TripId exists
        // We are therefore receiving a second or subsequent update for an existing trip,
        // and will store it in chronological order relative to others
        // This will be rare when parsing a single file
        // But if using multiple files, will see many subsequent updates, and many duplicates too!
        // This is because trains only provide updates when in motion, which is unpredictable
        // We have to dedupe them

        // Let's insert the record into the correct spot in the vector
        LOG4CXX_DEBUG(tripmap_logger, "Adding additional trip info for existing trip_id: " << new_ti.trip_id);
        TripInfoVec& tiv_existing = k_trip_map[new_ti.trip_id];

        // Find correct spot
        // Guaranteed at least 1 item here
        TripInfoVec::iterator itr = tiv_existing.begin();
        TripInfo tmp;
        while (itr != tiv_existing.end())
        {
            tmp = *itr;
            // Check if proposed record has newer timestamp than first record in the TIV
            if (tmp.pi.timestamp < new_ti.pi.timestamp)
            {
                LOG4CXX_DEBUG(tripmap_logger, "add_trip(): Inserting at front: new_ti is more recent than most recent existing ti: \n\t" << tmp << "\n\tvs new_ti:\n\t" << new_ti);
                tiv_existing.insert(itr, new_ti);
                return true;
            } 
            else if (tmp.pi.timestamp == new_ti.pi.timestamp)
            { 
                // Records have the same exact timestamp, likely duplicate record, so check it
                if (new_ti == tmp){
                    LOG4CXX_DEBUG(tripmap_logger, "add_trip(): Duplicate trip, discarding\n" 
                            << "tmp:\t" << tmp << "\nnew:\t" << new_ti);
                    return false;
                } else {
                    // Should never happen, but just in case...going to log it
                    LOG4CXX_DEBUG(tripmap_logger, "add_trip(): Timestamps match, but TripInfo structs are different!\n\t" << "tmp:\t" << tmp << "\nnew:\t" << new_ti);
                    itr++;
                }
            } 
            else 
            {
                // new_ti is just at an older timestamp than this record, so progress onward
                // Iter to next location, this isn't where we should insert
                LOG4CXX_DEBUG(tripmap_logger, "add_trip(): Additional TripInfo: itering again on timestamp:\n" 
                          << "tmp:\t" << tmp << "\nnew:\t" << new_ti);
                itr++;
            }

        }
        LOG4CXX_DEBUG(tripmap_logger, "add_trip(): Inserting at end, iteration complete: \n\t" << tmp << "\n\tfor new_ti:\n\t" << new_ti);
        // Will have not itered if it is newest update
        tiv_existing.insert(itr, new_ti);

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
