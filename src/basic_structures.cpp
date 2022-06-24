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

std::ostream& operator<<(std::ostream& os, std::vector<TripInfo>& tiv){
    std::stringstream ss;
    for (int i = 0; i < tiv.size(); i++){
        ss << "\n\t" << tiv[i];
    }
    os << ss.str() << "\n";
    return os;
}
