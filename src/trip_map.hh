// Structures used for other components 
#ifndef MTA_TRIP_MAP
#define MTA_TRIP_MAP

#include <stdlib.h>

#include <map>
#include <string>
#include <vector>

#include "basic_structures.hh"

class TripMap{
    public:
        bool add_trip(TripInfo);
        void add_trips(std::vector<TripInfo>* trips);
        int size(void){
            return k_trip_map.size();
        }
    private:
        std::map<std::string, TripInfo> k_trip_map;
};


#endif // MTA_TRIP_MAP header guard
