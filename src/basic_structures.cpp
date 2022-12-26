// definition of basic structures

#include <iomanip>
#include <sstream>
#include <ostream>

#include "basic_structures.hh"
#include "time_parser.hh"

std::ostream& operator<<(std::ostream& os, TripInfo& ti){
    std::stringstream ss;
    ss << (ti.is_assigned ? "+ " : "- ")
       << "Train: " << ti.trip_id << ", " << ti.direction << " " << ti.train_line 
       << " train, departed at: " 
       << std::setfill('0') << std::setw(2) << ti.hours << ":" 
       << std::setfill('0') << std::setw(2) << ti.minutes
       << ". Current position: " << ti.pi.current_stop
       << " @ " << get_time(ti.pi.timestamp);
    os << ss.str();
    return os;
}

std::ostream& operator<<(std::ostream& os, TripInfoVec& tiv){
    std::stringstream ss;
    for (int i = 0; i < tiv.size(); i++){
        ss << "\n\t" << tiv[i];
    }
    os << ss.str() << "\n";
    return os;
}

bool operator==(TripInfo& left, TripInfo& right){
    return left.trip_id == right.trip_id && 
        left.direction == right.direction && 
        left.train_line == right.train_line && 
        left.hours == right.hours && 
        left.minutes == right.minutes && 
        left.str_hours_minutes == right.str_hours_minutes && 
        left.is_assigned == right.is_assigned && 
        left.direction == right.direction && 
        left.pi.current_stop == right.pi.current_stop && 
        left.pi.timestamp == right.pi.timestamp;
}

bool operator!=(TripInfo& left, TripInfo& right){
    return !(left == right);
}
