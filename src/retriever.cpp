// Testing out accessing the web via cpp

#include <cstdio>
#include <ctime>

#include <string>
#include <vector>

#include "retriever.hh"


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
		output_file = fopen(iter->second.c_str(), "wb");
		if (!output_file)
		{
			curl_easy_cleanup(handle);
			std::cerr << "Error opening file: " << iter->second << std::endl;
			return false;
		}

        // set up the source, destination, and API Key
        // This is done on a per-connection / per-header basis, so can't be done sooner
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, output_file);
        curl_easy_setopt(handle, CURLOPT_URL, iter->first.c_str());
        
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
        std::cout << "Getting data to file: " << iter->second << '\n';
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

std::vector<std::string> get_mta_data()
{
    // Insert filenames into the vector given as we gather files
    std::vector<std::string> output_files;

    // instantiation
    CURL* handle;
    curl_global_init(CURL_GLOBAL_ALL);
    handle = curl_easy_init();

    // some option setting
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);

    // create the map of url:filename
    StringMap url_and_outfile;

    std::vector<std::string> extensions;
    extensions.push_back("ace");
    extensions.push_back("bdfm");
    extensions.push_back("g");
    extensions.push_back("jz");
    extensions.push_back("nqrw");
    extensions.push_back("l");
    extensions.push_back("si");
    extensions.push_back("1234567");

    // Fill it
    std::string url;
    url = "https://api-endpoint.mta.info/Dataservice/mtagtfsfeeds/nyct%2Fgtfs";
    std::string outfile = "data/gtfs_data_";
    std::string tmpurl;
    std::string tmpoutfile;

    for (int i = 0; i < extensions.size(); i++)
    {
        if (extensions[i] == std::string("1234567"))
        {
            // special case for 1234567, url does not have a '-' after gtfs
            tmpurl = url;
            tmpurl.append(extensions[i]);

            tmpoutfile = outfile;
            tmpoutfile.append(extensions[i]);
        }
        else
        {
            // this is no 1234567, so add a dash
            tmpurl = url;
            tmpurl.append("-");
            tmpurl.append(extensions[i]);

            // no dash for outfile though
            tmpoutfile = outfile;
            tmpoutfile.append(extensions[i]);

        }

        // Get time for file
        // YYYYMMDDHHMMSS date for filename
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer[80];

        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);

        std::strftime(buffer, sizeof(buffer),"%Y%m%d%H%M%S", timeinfo);
        std::string curtime(buffer);

        tmpoutfile.append("_");
        tmpoutfile.append(curtime);

        // store it in the output vector
        output_files.push_back(tmpoutfile);

        // and in the map that will be used to retrieve data
        // BUG: something is wrong here. Possibly writing different value to same key?
        url_and_outfile.insert(StringMapPair(tmpurl, tmpoutfile));
    }
    
    // do the thing	
    bool rc = get_data_for_mta_sites(&url_and_outfile, handle);

    // clean up
    curl_easy_cleanup(handle);

    if (rc == false)
    {
        std::cerr << "Problem during getting data for sites!" << std::endl;
    }

    return output_files;
}

