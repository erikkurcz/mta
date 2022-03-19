// Implementation for the file parser

#include "file_parser.hh"

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
            // NYCT uses the gfts protocol in its own way, so iter the messages and do some stuff to get the data
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
                    std::cout << "TripID: " << tdes.trip_id() << std::endl;
                    num_trips++;
                }
            }
        } // end iteration over realtime file
        
        // TODO: decided what to do with the below, presently unused and might not need to be
        // We don't necessarily need JSON, can just use C++ structures etc
        // Maybe will need to create our own structures of what we care about for subsequent analysis 
        // good to go from here
        //std::string json;
        //google::protobuf::util::MessageToJsonString(trip_descriptions, &json);
        //std::cout << json << std::endl;
        std::cout << "Num trips: " << num_trips << std::endl;
    }
    else 
    {
        std::cerr << "k_filepath has no size! Nothing to parse. k_filepath: '" << k_filepath << "'" << std::endl;
        return false;
    }
	return true;	
}

