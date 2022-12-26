// File parser for protobuf objects

#ifndef MTA_FILE_PARSER
#define MTA_FILE_PARSER

#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <fstream>

#include <curl/curl.h>
//#include <google/protobuf/util/json_util.h>

#include "protobufs/gtfs-realtime.pb.h"
#include "protobufs/nyct-subway.pb.h"
#include "static_data_parser.hh"
#include "utils.hh"
#include "basic_structures.hh"
//#include "utils/utils.hh"

class FileParser {

    public:
        bool set_filepath(std::string filepath){
            k_filepath = filepath;
            return true;
        }
        std::string get_filepath(){
            return k_filepath;
        }
        bool parse_file(StaticData* sd);
        std::vector<TripInfo>* get_all_trips()
        {
            return &k_all_trips;
        }
    private:
        std::string k_filepath;
        std::vector<TripInfo> k_all_trips;
};

#endif // MTA_FILE_PARSER header guard
