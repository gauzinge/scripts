#!/bin/bash

RUNSET="603 605 606 607 608 609"
FILES="";
for RUN in $RUNSET; do
FILE="/datadisk/Data/closed/USC.00000$RUN*"

echo 'Appending Data file for Run ' $RUN 
FILES=$FILE" $FILES"
done
echo 'copying to /afs/cern.ch/user/g/gauzinge/tb_data/'
echo
echo scp "xtaldaq@cmsuptracker003.cern.ch:$FILES" /afs/cern.ch/user/g/gauzinge/tb_data
scp -f "xtaldaq@cmsuptracker003.cern.ch:$FILES" /afs/cern.ch/user/g/gauzinge/tb_data
echo 'Don t forget to harvest RESULTS and remove native Data'
