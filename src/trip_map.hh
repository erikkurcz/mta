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
        TripInfoVec get_trips(std::string tripid){
            if (k_trip_map.count(tripid) == 0)
            {
                TripInfoVec tmp;
                return tmp;
            }
            else
            {
                return k_trip_map[tripid];
            }
        }
        typedef std::map<std::string, std::vector<TripInfo>>::iterator TripMapIterator;
        TripMapIterator begin() {
            return k_trip_map.begin();
        }
        TripMapIterator end(){
            return k_trip_map.end();
        }


    private:
        std::map<std::string, std::vector<TripInfo>> k_trip_map;
};


#endif // MTA_TRIP_MAP header guard
