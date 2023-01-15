// Implement query interface to get input from CLI user

#include <iostream>
#include <map>
#include <string>

#include "trip_map.hh"


bool query_tripmap(TripMap& tm)
{

    // Now that the stuff is loaded, let's allow the user to query
    // I guess we'll need an index at some point, 
    // but for now let's go on the basis of stations and just iter the TripMap
    // We can optimize for read later on, and have map of stations -> trains in the future
    // Or even a map of stations -> expected trains in N minutes (which is really what is useful)
    // But for now: simple query
    std::map<std::string, TripInfoVec> station_to_trips;
    std::map<std::string, TripInfoVec>::iterator stt;
    
    std::string input;
    TripMap::TripMapIterator it;
    it = tm.begin();
    std::cout << "Enter serch query (or 'quit'): " << std::endl;
    while (std::getline(std::cin, input))
    {
        // While we read input fine and it's not 'quit'
        // continue taking search terms
        if (input.compare("quit") == 0)
        {
            std::cout << "goodbye!" << std::endl;
            break;
        }

        // Print the results and clear results for next search
        std::cout << "Results for search term: " << input << "\n";
        while (it != tm.end())
        {
            // magic number: 0 index is most recent update
            // Make map of station->[tiv] so we can distinguish when printing
            if (it->second[0].pi.current_stop.find(input) != std::string::npos) 
            {
                std::string cstop_id = it->second[0].pi.current_stop_id;
                // Insert to map 
                stt = station_to_trips.find(cstop_id);
                if (stt == station_to_trips.end())
                {
                    // Not found, insert it
                    TripInfoVec tiv;
                    tiv.push_back(it->second[0]);
                    station_to_trips.insert(std::pair<std::string, TripInfoVec>(cstop_id,tiv));
                } 
                else
                {
                    // Found it, just append
                    stt->second.push_back(it->second[0]);
                }
                
            }
            it++;
        }
        
        // Iter the station_to_trips map and dump output
        stt = station_to_trips.begin();
        while (stt != station_to_trips.end())
        {
            for (int i = 0; i < stt->second.size(); i++)
            {
                std::cout << '\t' << stt->second[i] << '\n';
            }
            stt++;
        }

        // Clear out input
        station_to_trips.clear();
        input.clear();
        it = tm.begin();
        std::cout << "Enter serch query (or 'quit'): " << std::endl;
    }
    return true;
}
