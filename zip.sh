#!/bin/bash

SRC=$1  # for example ZPGine/src  or Client/
SEGMENTS=$2  # how many zip files

echo "Zipping $SRC into $SEGMENTS segments..."

ENGINE=$(ls $SRC)

ENGINE_SUB_COUNT=$(echo $ENGINE | wc -w)

echo $ENGINE_SUB_COUNT

COUNT=$(echo "($ENGINE_SUB_COUNT + 1) / $SEGMENTS" | bc)

echo "COUNT: $COUNT"

i=0
j=1

for item in $ENGINE; do
    $(echo "")

    echo "zip -r ZPG_SEGMENT$j.zip ./$SRC/$item"
    zip -r ZPG_SEGMENT$j.zip ./$SRC/$item

    i=$(echo "$i + 1" | bc);

    if [ "$(echo "$i % $COUNT" | bc)" -eq "0" ]; then
        j=$(echo "$j + 1" | bc)
    fi

done

