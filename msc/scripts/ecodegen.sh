#!/bin/bash
INC=../../include
cd $(dirname ${BASH_SOURCE[0]})
i=0
while read line; do
    echo $line | sed "s/{.*\(\<.\+\>\),.\+/\1 = $i,/g"
    i=$(expr $i + 1)
done < $INC/data/err_table.h > $INC/data/err_codes.h
