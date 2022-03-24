// Parser for mta_static_data/*txt files

#ifndef MTA_STATIC_DATA_PARSER_HH
#define MTA_STATIC_DATA_PARSER_HH

#include <fstream>
#include <string>
#include <map>

typedef std::string StopId;
typedef std::string StopName;
typedef std::map<StopId, StopName> StopIdNameMap;

StopIdNameMap* get_stop_map(std::string filename);

class StaticData{
    public:
        void set_stops_txt_filename(std::string filename){
            k_stops_filename = filename;
        }
        StopName get_stop_name(StopId sid);
        void initialize();
    private:
        std::string k_stops_filename;
        StopIdNameMap* stopmap_ptr; 
};


#endif // MTA_STATIC_DATA_PARSER_HH
