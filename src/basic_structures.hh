// Structures used for other components 
#ifndef MTA_BASIC_STRUCTURES
#define MTA_BASIC_STRUCTURES

#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>


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

std::ostream& operator<<(std::ostream& os, TripInfo& ti);
std::ostream& operator<<(std::ostream& os, std::vector<TripInfo>& tiv);

#endif // MTA_BASIC_STRUCTURES header guard
