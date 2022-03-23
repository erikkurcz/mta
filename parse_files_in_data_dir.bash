#!/bin/bash

rm -f current_status_all_lines

for file in $(ls data)
do 
    ./mta -f data/$file >> current_status_all_lines
done

