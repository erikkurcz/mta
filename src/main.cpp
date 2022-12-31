// Main for running this application 
#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"

#include "static_data_parser.hh"
#include "file_parser.hh"
#include "trip_map.hh"
#include "utils.hh"

struct cli_args {
	std::string station;
	std::string direction;
	std::string route;	
    std::vector<std::string> filenames;	
	std::string stops_txt_filename;	
};

static void show_usage(void)
{
	std::cerr << "Usage:\nmta -s|--station []\n"
		  << "-d|--direction [north|south]\n"
		  << "-r|--route [1|2|3|4|5|6|7|S|A|C|E|B|D|F|M|J|Z|G ... \n"
          << "-f|--filename FILENAME]\n"
          << "[-f|--filename FILENAME] ... \n"
		  << std::endl; 
}

// Set up logger
log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("mta_app"));

int main(int argc, char* argv[])
{
	cli_args args;
	
	// sanity check	
	if (argc <= 1)
	{
		show_usage();
		return 1; 
	}
	
	// parse out	
	int c;	
	while( (c = getopt(argc, argv, "s:d:r:f:")) != -1 )
	{
		switch (c)
		{
			/*case 's':
				args.station = optarg;
				break;*/
			case 'd':
				args.direction = optarg;
				break;
			case 'r':
				args.route = optarg;
				break;
			case 'f':
                args.filenames.push_back(optarg);
				break;
			case 's':
                // TODO: default this out, or use config maybe?  
                args.stops_txt_filename = optarg;
				break;
		}
	}

    log4cxx::BasicConfigurator::configure();
	LOG4CXX_INFO(logger, "Test");
    std::stringstream msg;
    msg << "Arguments given:"
        << "\nStation: " << args.station
        << "\nDirection: " << args.direction
        << "\nRoute: " << args.route
        << "\nFilenames: " << args.filenames
        << "\nStops.txt filename: " << args.stops_txt_filename << std::endl;
    LOG4CXX_INFO(logger, msg.str());

    // Get files from MTA here if no filename given
    //if (!args.filename)
    //{
    //    // Get data from MTA here
    //    std::cout << "Getting data from MTA..." << std::endl;
    //}

    // Parse stops.txt
    StaticData sd;
    sd.set_stops_txt_filename(args.stops_txt_filename);
    sd.initialize();
    
    // Will contain all trips for all files
    TripMap tm;
    std::cout << "Beginning to parse files, TripMap size: " << tm.size() << std::endl;

	// parse file(s) here
    // quick hack
    std::string a_trip_id;
    while (args.filenames.size() > 0)
    {
        std::string tmp_filename = args.filenames.back();
        
        FileParser fp;
        fp.set_filepath(tmp_filename);
        if (!fp.parse_file(&sd)){
            std::cerr << "Parsing file: " << tmp_filename << ", Failed to parse" << std::endl;
            return 1;
        }

        // Add to a data structure here and do some work?
        std::vector<TripInfo>* trips = fp.get_all_trips();
        std::cout << "Starting to parse file: " << tmp_filename << ", got " << trips->size() << " updates" << std::endl;

        // Put Trips into map
        a_trip_id = (*trips)[0].trip_id;
        tm.add_trips(trips);
        std::cout << "Done parsing file: " << tmp_filename << ", TripMap size: " << tm.size() << std::endl;

        args.filenames.pop_back();

    }

    // TripMap is string is unique {tripid:[vector of trip updates]}
    // We can iter it and find trips with > 1 update and dump their updates in chrono order!
    TripMap::TripMapIterator it;
    it = tm.begin();
    while (it != tm.end())
    {
        if (it->second.size() > 1)
        {
            std::cout << "Found trip with >1 update:\n" << std::endl;
            for (int i = 0; i < it->second.size(); i++)
            {
                std::cout << it->second[i] << std::endl;
            }
        }
        it++;
    }


    std::cout << "Done parsing files, TripMap size: " << tm.size() << std::endl;

    std::cout << "Dumping single trip: " << a_trip_id << std::endl;
    TripInfoVec vec_of_a_trip = tm.get_trips(a_trip_id);
    std::cout << vec_of_a_trip << std::endl;


	return 0;
}

