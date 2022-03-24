#!/bin/bash

rm -f current_status_all_lines

for file in $(ls data)
do 
    ./mta -f data/$file -s mta_static_data/stops.txt >> current_status_all_lines
done

