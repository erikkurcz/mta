// Main for running this application 
#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

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

	std::cout << "Arguments given:"
		  << "\nStation: " << args.station
		  << "\nDirection: " << args.direction
		  << "\nRoute: " << args.route
		  << "\nFilenames: " << args.filenames
		  << "\nStops.txt filename: " << args.stops_txt_filename
		  << std::endl;

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
    while (args.filenames.size() > 0)
    {
        std::string tmp_filename = args.filenames.back();
        
        std::cout << "Parsing file: " << tmp_filename << std::endl;
        FileParser fp;
        fp.set_filepath(tmp_filename);
        if (!fp.parse_file(&sd)){
            std::cerr << "Parsing file: " << tmp_filename << ", Failed to parse" << std::endl;
            return 1;
        }

        // Add to a data structure here and do some work?
        std::vector<TripInfo>* trips = fp.get_all_trips();
        std::cout << "Parsing file: " << tmp_filename << ", got " << trips->size() << " trips" << std::endl;

        // Put Trips into map
        tm.add_trips(trips);
        std::cout << "Parsing file: " << tmp_filename << ", TripMap size: " << tm.size() << std::endl;

        args.filenames.pop_back();

    }

    std::cout << "Done parsing files, TripMap size: " << tm.size() << std::endl;
	return 0;
}

