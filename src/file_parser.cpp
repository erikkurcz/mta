// Implementation for the file parser

#include <stdlib.h>

//#include <regex> // use this one day
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

#include "file_parser.hh"

struct TripInfo {
    std::string direction;
    std::string train_line;
    int hours;
    int minutes;
    std::string str_hours_minutes;
};

std::ostream& operator<<(std::ostream& os, TripInfo& ti){
    std::stringstream ss;
    ss << "Train: " << ti.direction << " " << ti.train_line 
       << " train, departed at: " 
       << std::setfill('0') << std::setw(2) << ti.hours << ":" 
       << std::setfill('0') << std::setw(2) << ti.minutes;
    os << ss.str();
    return os;
}

bool sortDepartureTime(const TripInfo& a, const TripInfo& b){
    if (a.hours == b.hours){
        return a.minutes < b.minutes;
    }
    return a.hours < b.hours;
}

bool get_TripInfo(std::string str, TripInfo& ti){
    
    // Parses the raw trip ID into something human-readable
    // Per https://api.mta.info/GTFS.pdf
    // Initial 6 digits indicate origin time
        // Times are coded reflecting hundredths of a minute past midnight
        // '+' indicates 30 seconds
        // Rounding to nearest minute is acceptable as this is more precise than a transit operation can actually operate
        // Negative numbers reflect times prior to the day of the schedule
        // Numbers in excess of '00144000' (1440 minutes per day) reflect times beyond the day of schedule
        // 00145000 indicates 12:10AM tomorrow
    // Underscore is a separator
    // "2..N08R" identifies trip path (stopping pattern) for a unique train trip
        // Decomposing: 2 -> 2 train; N -> Direction (North), 08R -> Path identifier
        // Path identifier encodes origin, destination, stops, express/local, operating time periods, and shape (circle / diamond).
        // Path identifier is OPTIONAL is may not always be present, it may be present in some messages and not present in others

    // ******* NOTE *******
    // There is almost certainly a library that does this 
    // Need to replace this in the future, as this is extremly brittle
    // ******* NOTE *******

    // Onto the parsing per above rules
    
    // origin departure time info
    // remember that it's stored as HUNDREDTHS of minutes past midnight, so convert
    float minutes_past_midnight = atoi(str.substr(0,6).c_str()) / 100;
    float hours_past_midnight = floor(minutes_past_midnight / 60);
    float minutes_past_hour = minutes_past_midnight - (60*hours_past_midnight);
    std::stringstream hours_and_minutes;
    hours_and_minutes << std::setfill('0') << std::setw(2) << hours_past_midnight;
    hours_and_minutes << std::setfill('0') << std::setw(2) << minutes_past_hour;

    // Train line
    std::string train_line(str.substr(7,1));

    // Direction
    std::string train_direction(str.substr(str.size()-1,1));
    std::string dir = train_direction == std::string("N") ? "Northbound" : "Southbound";

    ti.direction = dir;
    ti.train_line = train_line;
    ti.hours = hours_past_midnight;
    ti.minutes = minutes_past_hour;
    ti.str_hours_minutes = hours_and_minutes.str();
    
    return true;
}

// Parse file into protobuf objects
bool FileParser::parse_file(void)
{

    if (k_filepath.size()){

        // Parse file of gtfs data in order to work with protobuf 
        std::fstream data(k_filepath.c_str(), std::ios::in | std::ios::binary);

        if (!data)
        {
            std::cerr << "Error opening file: " << k_filepath << ", error: " << strerror(errno) << std::endl;
            return false;
        }
        
        transit_realtime::FeedMessage fmessage;
        std::vector<TripInfo> all_trips;
        if (!fmessage.ParseFromIstream(&data))
        {
            std::cerr << "Error parsing file: " << k_filepath << ", error: " << strerror(errno) << std::endl;
            return false;
        }
        
        // Iter the messages within the file
        // TODO: really this should parse data into structures we care about, but for now just print out what we care about
        int num_trips = 0;
        for (int iter = 0; iter < fmessage.entity_size(); iter++)
        {
            // NYCT uses the gfts protocolin its own way, so iter the messages and do some stuff to get the data
            // TripUpdate are mostly what we care about - NYCT defines these as those underway or scheduled in the next 30 minutes
            const transit_realtime::FeedEntity& fentity = fmessage.entity(iter);

            if (fentity.has_vehicle()){
                // `vehicle` is a VehiclePosition, which is present when a vehicle is in motion
                // assigned trains that haven't moved yet are not present
                // Something we care about, parse it out
                const transit_realtime::VehiclePosition& vehicle = fentity.vehicle();
                
                // VehiclePositions have TripDescriptors present
                if (vehicle.has_trip()){
                    const transit_realtime::TripDescriptor tdes = vehicle.trip();

                    // We have trip info at this point, let's print some stuff out
                    TripInfo ti;
                    get_TripInfo(tdes.trip_id(), ti);
                    all_trips.push_back(ti);

                }
            }
        } // end iteration over realtime file

        // sort the trips based on departure time
        // then iter and dump them out
        std::sort(all_trips.begin(), all_trips.end(), sortDepartureTime);

        for (int i = 0; i < all_trips.size(); i++){
            std::cout << all_trips[i] << std::endl;
        }
        
        // TODO: decided what to do with the below, presently unused and might not need to be
        // We don't necessarily need JSON, can just use C++ structures etc
        // Maybe will need to create our own structures of what we care about for subsequent analysis 
        // good to go from here
        //std::string json;
        //google::protobuf::util::MessageToJsonString(trip_descriptions, &json);
        //std::cout << json << std::endl;
        std::cout << "Num trips in this file: " << all_trips.size() << std::endl;
    }
    else 
    {
        std::cerr << "k_filepath has no size! Nothing to parse. k_filepath: '" << k_filepath << "'" << std::endl;
        return false;
    }
	return true;	
}

