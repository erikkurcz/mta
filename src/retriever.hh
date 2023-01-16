#ifndef MTA_RETRIEVER
#define MTA_RETRIEVER

// Testing out accessing the web via cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <map>

#include <curl/curl.h>

typedef std::map<const char*, const char*> StringMap;
typedef std::pair<const char*, const char*> StringMapPair;
typedef std::map<const char*, const char*>::iterator StringMapIter;

static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream);

bool get_data_for_mta_sites(StringMap* map_ptr, CURL*& handle);

bool get_mta_data(void);


#endif // MTA_RETRIEVER header guard
