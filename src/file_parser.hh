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

//#include "protobufs/nyct-subway.pb.h"
#include "protobufs/gtfs-realtime.pb.h"
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
        bool parse_file(void);
    private:
        std::string k_filepath;
};

#endif // MTA_FILE_PARSER header guard
