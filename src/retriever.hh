#ifndef MTA_RETRIEVER
#define MTA_RETRIEVER

// Testing out accessing the web via cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <curl/curl.h>

typedef std::map<std::string, std::string> StringMap;
typedef std::pair<std::string, std::string> StringMapPair;
typedef std::map<std::string, std::string>::iterator StringMapIter;

static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream);

bool get_data_for_mta_sites(StringMap* map_ptr, CURL*& handle);

std::vector<std::string> get_mta_data();


#endif // MTA_RETRIEVER header guard
