#!/bin/bash
for i in {2..1000000}
do
  echo "$i"
  first=$((i/2))
  second=$((i-$first))
  ./inplacemerge sources/test.txt $first $second > /dev/null
  ./inplacemerge sources/test.txt | grep "failing" > err
  if [ -s err ]
  then
    echo "Error in: $i"
  fi
done
echo "Ended"
