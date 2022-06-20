// Implementation for the file parser

#include <stdlib.h>

//#include <regex> // use this one day
#include <algorithm>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

#include "file_parser.hh"


// Parse file into protobuf objects
bool FileParser::parse_file(StaticData* sd)
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
            
            // VehiclePosition parsing
            if (fentity.has_vehicle()){
                // `vehicle` is a VehiclePosition, which is present when a vehicle is in motion
                // assigned trains that haven't moved yet are not present
                // Something we care about, parse it out
                const transit_realtime::VehiclePosition& vehicle_pos = fentity.vehicle();
                
                // VehiclePositions have TripDescriptors present
                // TripDescriptor parsing 
                // TripDescriptor describes the overall trip ie start time and origin, direction, the like
                if (vehicle_pos.has_trip()){
                    const transit_realtime::TripDescriptor tdes = vehicle_pos.trip();

                    // We have trip info at this point, let's print some stuff out
                    TripInfo ti;
                    PositionInfo pi;
                    ti.pi = pi;
                    get_TripInfo(tdes.trip_id(), ti);

                    // Check if there is the nyct_trip_descriptor Extension, which will tell us if a trip is assigned, among other info
                    if (tdes.HasExtension(nyct_trip_descriptor)){
                        NyctTripDescriptor nyct_tdes = tdes.GetExtension(nyct_trip_descriptor);
                        ti.is_assigned = nyct_tdes.has_is_assigned() ? nyct_tdes.is_assigned() : false;
                    }

                    // Parse out StopTimeUpdate
                    // StopTimeUpdate indicates *current stop*
                    // Combine this with timestamp to know where it is and when
                    if (vehicle_pos.has_stop_id()){
                        // Train is at a station, parse where it is and the timestamp
                        ti.pi.current_stop = sd->get_stop_name(vehicle_pos.stop_id());
                        ti.pi.timestamp = vehicle_pos.has_timestamp() ? vehicle_pos.timestamp() : -1;
                    }
                    k_all_trips.push_back(ti);
                }
                

                // TripUpdate parsing for this same Trip!
            }
        } // end iteration over realtime file

        // sort the trips based on departure time
        // then iter and dump them out
        std::sort(k_all_trips.begin(), k_all_trips.end(), sortDepartureTime);

        for (int i = 0; i < k_all_trips.size(); i++){
            std::cout << k_all_trips[i] << std::endl;
        }
        
        // TODO: decided what to do with the below, presently unused and might not need to be
        // We don't necessarily need JSON, can just use C++ structures etc
        // Maybe will need to create our own structures of what we care about for subsequent analysis 
        // good to go from here
        //std::string json;
        //google::protobuf::util::MessageToJsonString(trip_descriptions, &json);
        //std::cout << json << std::endl;
        std::cout << "Num trips in this file: " << k_all_trips.size() << std::endl;
    }
    else 
    {
        std::cerr << "k_filepath has no size! Nothing to parse. k_filepath: '" << k_filepath << "'" << std::endl;
        return false;
    }
	return true;	
}

