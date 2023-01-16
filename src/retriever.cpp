// Testing out accessing the web via cpp

#include "retriever.hh"

typedef std::map<const char*, const char*> StringMap;
typedef std::pair<const char*, const char*> StringMapPair;
typedef std::map<const char*, const char*>::iterator StringMapIter;

static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
	return written;
}

bool get_data_for_mta_sites(StringMap* map_ptr, CURL*& handle)
{
    // Assumes handle is read to go 

	// basic setup 
	FILE* output_file; 
	CURLcode rc;

	// create iterator to beginning of map
    StringMapIter iter;
	iter = map_ptr->begin();

	// iter and make calls
	while (iter != map_ptr->end())
	{
		// open output file
		output_file = fopen(iter->second, "wb");
		if (!output_file)
		{
			curl_easy_cleanup(handle);
			std::cerr << "Error opening file: " << iter->second << std::endl;
			return false;
		}

        // set up the source, destination, and API Key
        // This is done on a per-connection / per-header basis, so can't be done sooner
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, output_file);
        curl_easy_setopt(handle, CURLOPT_URL, iter->first);
        
        // Set up api key in header
        struct curl_slist* header = NULL;
        std::string kv("x-api-key:");
        kv.append(getenv("MTAKEY"));

        header = curl_slist_append(header, kv.c_str());
        if (header == NULL)
        {
            std::cerr << "problem appending to header!" << std::endl;
            // close out and flush
            fclose(output_file);
            return false;
        }
        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header);

        // Do the call
		rc = curl_easy_perform(handle);

		// close out and flush
		fclose(output_file);

        // Error check
        if (rc != CURLE_OK)
        {
			std::cerr << "Something went wrong for: " << iter->first << " : " << curl_easy_strerror(rc) << std::endl;
            std::cerr << "curl_easy_perform failed: " << curl_easy_strerror(rc) << std::endl;
            std::cerr << "Status code for site: " << iter->first << ": " << curl_easy_strerror(rc) << std::endl;
            return false;
        }
        else 
        {
			std::cout << "Data successfully retrieved for: " << iter->first << std::endl;
        }
	
		iter++;
	}
    return true;
}

bool get_mta_data(void)
{
    // instantiation
    CURL* handle;
    curl_global_init(CURL_GLOBAL_ALL);
    handle = curl_easy_init();

    // some option setting
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);

    // create the map of url:filename
    StringMap url_and_outfile;
    url_and_outfile.insert(StringMapPair("https://api-endpoint.mta.info/Dataservice/mtagtfsfeeds/nyct%2Fgtfs-bdfm", "gtfs.bfdm.data"));

    // do the thing	
    bool rc = get_data_for_mta_sites(&url_and_outfile, handle);

    // clean up
    curl_easy_cleanup(handle);

    if (rc == false)
    {
        std::cerr << "Problem during getting data for sites!" << std::endl;
    }
    return rc == true;
}

