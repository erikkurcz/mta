#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>

#include "static_data_parser.hh"

StopIdNameMap* get_stop_map(std::string filename){
    // Parses stops.txt file, given as a filename to the function
    // Returns pointer to map of StopId->StopName
    // May be expanded to include all data for stations later on (lat/lon/parent station, etc)
    StopIdNameMap* map_ptr = new StopIdNameMap;
    
    char tmpline_c[512];
    std::string tmpline;
    std::string tmp_id;
    std::string tmp_name;

    std::ifstream infile(filename);
    while (infile.good()){
        
        // get the whole line into a string
        infile.getline(tmpline_c, 512);
        tmpline = std::string(tmpline_c);

        // erase \r and \n
        size_t pos = tmpline.find('\r');
        if (pos != std::string::npos){
            tmpline.erase(pos, 1);
        }
        pos = tmpline.find('\n');
        if (pos != std::string::npos){
            tmpline.erase(pos, 1);
        }

        // get first term in comma-delimited string, which is the tmp_id
        // ex: 101,,Van Cortlandt Park-242 St,...
        size_t first_comma = tmpline.find(',');
        size_t second_comma = tmpline.find(',', first_comma+1);
        size_t third_comma = tmpline.find(',', second_comma+1);
        
        // Args are (position, len)
        tmp_id = tmpline.substr(0, first_comma);
        tmp_name = tmpline.substr(second_comma+1,third_comma-second_comma-1);

        StopId stop_id(tmp_id);
        StopName stop_name(tmp_name);

        // use std::getline to parse the relevant fields
        map_ptr->insert(std::pair<StopId, StopName>(tmp_id, tmp_name));
    }

    return map_ptr;
}

void StaticData::initialize(){
    stopmap_ptr = get_stop_map(k_stops_filename);
}

StopName StaticData::get_stop_name(StopId sid){

    // We aren't necessarily guaranteed a StopID to be present here...
    // I've seen 'R60N' be given as a StopID, which is not present in stops.txt
    // Nor is it present on a newer version of static data from the MTA
    // So we'll return a blank StopName and require the caller to handle this
    // If the id isn't found

    // StopName is just a string anyway
    if (!stopmap_ptr->count(sid))
    {
        StopName sname("UNKNOWN: ");
        sname += sid;
        std::cerr << "ERROR: StopID '" << sid << "' not found in stops.txt!" << std::endl;
        return sname;
    }

    return stopmap_ptr->find(sid)->second;
}

