#!/bin/bash

rm -f current_status_all_lines

ALL_FILES=""
for file in $(ls data)
do 
    ALL_FILES="$ALL_FILES -f data/$file" 
done

echo "bash -c \"./mta $ALL_FILES -l log_config.txt -s mta_static_data/stops.txt >> current_status_all_lines\""
bash -c "./mta $ALL_FILES -l log_config.txt -s mta_static_data/stops.txt >> current_status_all_lines"

echo "done"
