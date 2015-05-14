#!/bin/bash
# todo- get the directory thing right, use better sed expression
INC=../../include
cd $(dirname ${BASH_SOURCE[0]})
i=0
while read line; do
    echo $line | sed "s/{.*\(\<.\+\>\),.\+/\1 = $i,/g"
    i=$(expr $i + 1)
done < $INC/data/error_table.h > $INC/data/error_codes.h
