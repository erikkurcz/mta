#!/bin/bash

# iter the line extensions and get files

for ext in ace bdfm g jz nqrw l si "" 
do
    NOW=$(date +%Y%m%d%H%M)
    filename_ext=$ext
    if [[ $ext == "" ]]
    then
        ext="$ext"
        filename_ext="1234567"
    else
        ext="-$ext"
    fi

    curl -s -H @headers --output data/gtfs_data_${filename_ext}_${NOW} https://api-endpoint.mta.info/Dataservice/mtagtfsfeeds/nyct%2Fgtfs${ext}
    sleep 1
    echo "Got data for ${filename_ext}"
done

