// Main for running this application 
#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include "file_parser.hh"
#include "structures.hh"
#include "static_data_parser.hh"

struct cli_args {
	std::string station;
	std::string direction;
	std::string route;	
	std::string filename;	
	std::string stops_txt_filename;	
};

static void show_usage(void)
{
	std::cerr << "Usage:\nmta -s|--station []\n"
		  << "-d|--direction [north|south]\n"
		  << "-r|--route [1|2|3|4|5|6|7|S|A|C|E|B|D|F|M|J|Z|G ... \n"
          << "-f|--filename FILENAME\n"
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
				args.filename = optarg;
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
		  << "\nFilename: " << args.filename
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

	// parse file here
    FileParser fp;
    fp.set_filepath(args.filename);
    if (!fp.parse_file(&sd)){
        std::cerr << "Failed to parse: " << args.filename << std::endl;
        return 1;
    }

    // Add to a data structure here and do some work?
    std::vector<TripInfo>* trips = fp.get_all_trips();

    // Put Trips into a map
    std::map<std::string, TripInfo> tinfo;

    for (int i = 0; i < trips->size(); i++){
        TripInfo& tmp = trips->at(i);
        if (tinfo.count(tmp.trip_id) == 0)
        {
            std::cout << "Adding trip info for trip_id: " << tmp.trip_id << std::endl;
            // Doesn't exist, so just add it
            tinfo[tmp.trip_id] = tmp;
        }
        else 
        {
            // Does exist
            // Let's update the existing if it is a more recent update
            TripInfo& existing = tinfo[tmp.trip_id];
            if (tmp.pi.timestamp > existing.pi.timestamp)
            {
                // We've come across a more recent update for this trip
                // Replace what exists in tinfo
                std::cout << "Found more recent update than existing:\n"
                          << "\t\tNEW: " << tmp 
                          << "\n\t\tvs\n"
                          << "\t\tOLD: " << existing << std::endl;
                tinfo[tmp.trip_id] = tmp;
            }
            else
            {
                // Just came across an older update than what exists, so no action needed
                std::cout << "Found old update, not replacing existing info\n"
                          << "\t\tEXISTING: " << existing 
                          << "\n\t\tvs\n"
                          << "\t\tOLD: " << tmp << std::endl;
            }
        }
    }

    // Map is updated with most recent info for all trips
    std::cout << "Map is updated, basic info:\n" 
              << "Total updates in file(s): " << trips->size() << "\n"
              << "Total unique positions in system: " << tinfo.size() << std::endl;
	return 0;
}

