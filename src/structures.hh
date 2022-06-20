// Structures used for other components 
#ifndef MTA_STRUCTURES
#define MTA_STRUCTURES

#include <stdlib.h>

#include <string>


struct PositionInfo{
    std::string current_stop;
    int timestamp;
};

struct TripInfo {
    std::string trip_id;
    std::string direction;
    std::string train_line;
    int hours;
    int minutes;
    std::string str_hours_minutes;
    PositionInfo pi;
    bool is_assigned;
};


#endif // MTA_STRUCTURES header guard
