#!/bin/bash

# iter the line extensions and get files
STARTSTRING="Got data for:"
OUTLINE=$STARTSTRING

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
    if [[ $OUTLINE == $STARTSTRING ]]
    then
        OUTLINE="$OUTLINE ${filename_ext}"
    else
        OUTLINE="$OUTLINE, ${filename_ext}"
    fi

    printf "\r$OUTLINE"
    
done
printf "\n"
