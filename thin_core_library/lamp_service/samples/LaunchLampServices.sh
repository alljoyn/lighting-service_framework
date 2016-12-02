#!/bin/bash

##
#Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
#Source Project Contributors and others.
#
#All rights reserved. This program and the accompanying materials are
#made available under the terms of the Apache License, Version 2.0
#which accompanies this distribution, and is available at
#http://www.apache.org/licenses/LICENSE-2.0

name="LampServiceDir"
specialChar="*"

rm -r -f $name$specialChar

echo "Please enter number of lamp services (Range: 1 to 100) to run:"
read numLampServices

# Initialize Pid List
pidLst=""

for (( i=1; i <= $numLampServices; i++ ))
do
   dname=$name$i
   mkdir "$dname"
   cd "$dname"
   ../lamp_service &
   pidLst="$pidLst $!"
   echo "Launched Lamp Service $i"
   cd ..
done

# trap ctrl-c and call ctrl_c()
trap ctrl_c INT
trap ctrl_z TSTP

function ctrl_c() {
  echo "Trapped CTRL-C"
  echo "Please press Enter to exit"
}

function ctrl_z() {
  echo "Trapped CTRL-Z"
  echo "Please press Enter to exit"
}

echo "Please press Enter to exit"
read exitKey

for pid in $pidLst
do
        ps | grep $pid
        kill -9 $pid
done

rm -r -f $name$specialChar


