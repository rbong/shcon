#!/bin/bash
cd $(dirname ${BASH_SOURCE[0]})
make
printf "\nSearching for unused error codes...\n"
../msc/scripts/ecodesrc.sh
if [ $? -eq 0 ]; then
    printf "No unused errror codes found.\n"
fi
printf "Done searching for error codes.\n\n"
./test-mm
printf "Done."
