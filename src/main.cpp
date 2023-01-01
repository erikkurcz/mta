// Main for running this application 
#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"

#include "static_data_parser.hh"
#include "file_parser.hh"
#include "trip_map.hh"
#include "utils.hh"

struct CLIArgs {
	std::string station;
	std::string direction;
	std::string route;	
	std::string log_config;	
    std::vector<std::string> filenames;	
	std::string stops_txt_filename;	
};

std::ostream& operator<<(std::ostream& os, const struct CLIArgs& args)
{
    os << "station = " << args.station << ", ";
    os << "direction = " << args.direction << ", ";
    os << "route = " << args.route << ", ";
    os << "stops_txt_filename = " << args.stops_txt_filename << ", ";
    os << "log_config = " << args.log_config << ", ";
    os << "data_filenames = ";

    for (int i = 0; i < args.filenames.size(); i++)
    {
        os << args.filenames[i] << " ";
    }
    return os;
}

static void show_usage(void)
{
	std::cerr << "Usage:\nmta -s|--station []\n"
		  << "-d|--direction [north|south]\n"
		  << "-r|--route [1|2|3|4|5|6|7|S|A|C|E|B|D|F|M|J|Z|G ... \n"
          << "-l|--log-config LOG_CONFIG_FILENAME\n"
          << "-f|--filename FILENAME\n"
          << "[-v|--verbose]"
		  << std::endl; 
}

log4cxx::LoggerPtr main_logger(log4cxx::Logger::getLogger("mta"));

int main(int argc, char* argv[])
{
	CLIArgs args;
	
	// sanity check	
	if (argc <= 1)
	{
		show_usage();
		return 1; 
	}

    bool set_log_debug(false);

	// parse out	
	int c;	
	while((c = getopt(argc, argv, "s:d:r:l:v::f:")) != -1 )
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
			case 'l':
                // Set up logger from file
				args.log_config = optarg;
                log4cxx::PropertyConfigurator::configure(args.log_config);
				break;
			case 'f':
                args.filenames.push_back(optarg);
				break;
			case 's':
                // TODO: default this out, or use config maybe?  
                args.stops_txt_filename = optarg;
				break;
			case 'v':
                set_log_debug = true;
				break;
		}
	}

    if (set_log_debug)
    {
        main_logger->setLevel(log4cxx::Level::getDebug());
    }

    LOG4CXX_INFO(main_logger, "Arguments given:" << args);
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
    LOG4CXX_INFO(main_logger, "Beginning to parse files, TripMap size: " << tm.size());

	// parse file(s) here
    // quick hack
    std::string a_trip_id;
    while (args.filenames.size() > 0)
    {
        std::string tmp_filename = args.filenames.back();
        
        FileParser fp;
        fp.set_filepath(tmp_filename);
        if (!fp.parse_file(&sd)){
            LOG4CXX_ERROR(main_logger, "Failed to parse: " << tmp_filename);
            return 1;
        }

        // Add to a data structure here and do some work?
        std::vector<TripInfo>* trips = fp.get_all_trips();
        LOG4CXX_DEBUG(main_logger, "Starting to parse file: " << tmp_filename << ", got " << trips->size() << " updates");

        // Put Trips into map
        a_trip_id = (*trips)[0].trip_id;
        tm.add_trips(trips);
        LOG4CXX_DEBUG(main_logger, "Done parsing file: " << tmp_filename << ", TripMap size: " << tm.size());

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
            LOG4CXX_INFO(main_logger, "Found trip with >1 update: " << it->second);
        }
        it++;
    }

    LOG4CXX_INFO(main_logger, "Done parsing files, TripMap size: " << tm.size());

    TripInfoVec vec_of_a_trip = tm.get_trips(a_trip_id);
    LOG4CXX_INFO(main_logger, "Dumping single trip, trip_id: " << a_trip_id << ", updates: " << vec_of_a_trip);


	return 0;
}

