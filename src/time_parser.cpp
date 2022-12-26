// Definitions of utilities 
#include "time_parser.hh"

std::string get_time(time_t unix_timestamp){

    if (unix_timestamp == -1){
        return std::string("<not reported>");
    }

    char time_buf[80];
    struct tm ts;
    ts = *localtime(&unix_timestamp);
    strftime(time_buf, sizeof(time_buf), "%m-%d-%Y %H:%M:%S %Z", &ts);
    return std::string(time_buf);
}

