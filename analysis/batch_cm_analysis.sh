#!/bin/bash

echo 'Submitter script for CM Analysis to LXBATCH!'
echo 'Usage: ./'$0 'CBC(AorB) input_files(including_*)'

CHIPS='A B'

if [ $1=="A" ]; then
	idx=0
	files=0
	for i in $*; do
		if [ $idx -eq 0 ]; then
			idx=$((idx+1))
			continue
		fi
		echo 'Submitting Analysis for file' $i', CBC '$1'!'
		echo 'root -b -q -l src/cm_analysis.cc+('"'$i'","'$1'"')'
		root -b -q -l 'src/cm_analysis.cc+("'$i'","'$1'")'
		files=$((files+1))
	done
elif [ $1=="B" ]; then
	idx=0
	files=0
	for i in $*; do
		if [ $idx -eq 0 ]; then
			idx=$((idx+1))
			continue
		fi
		echo 'Submitting Analysis for file' $i', CBC '$1'!'
		echo 'root -b -q -l src/cm_analysis.cc+('"'$i'","'$1'"')'
		root -b -q -l 'src/cm_analysis.cc+("'$i'","'$1'")'
		files=$((files+1))
	done
# elif [$1=="AB"]; then
# 	idx=0
# 	for i in $*; do
# 		if [ $idx -eq 0 ]; then
# 			idx=$((idx+1))
# 			continue
# 		fi
# 		# for CBC in $CHIPS; do
# 			echo 'Submitting Analysis for file' $i', CBC ''!'
# 			echo 'root -b -q -l src/cm_analysis.cc+('"'$i'"',A)'
# 			echo 'root -b -q -l src/cm_analysis.cc+('"'$i'"',B)'
# 			# root -b -q -l 'src/cm_analysis.cc+("'$i'","'$CBC'")'
# 		# done
# 	done
fi

echo 'Done processing '$files' input files!'
