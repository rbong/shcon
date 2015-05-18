#!/bin/bash
# todo- get the directory thing right, make a script to find unused codes
INC=../../include
SRC=../../src
ret=0
cd $(dirname ${BASH_SOURCE[0]})
(sed 's/{.*\(\<.\+\>\),.\+/\1/g' < $INC/data/err_table.h) > /tmp/codes
while read line; do
    grep -q -r $line $SRC
    if [ $? -ne 0 ]; then
        ret=1
        printf "%s:%d: Warning: error code %s is unused.\n" $0 $LINENO $line
    fi
done < /tmp/codes
exit $ret
