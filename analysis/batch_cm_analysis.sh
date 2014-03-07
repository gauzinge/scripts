#!/bin/bash

echo 'Setting Environment!'

# set the environment
export PLATFORM=x86_64-slc6
export GCC_VERSION=4.6
export GCC_VERSION_SHORT=46
export PYTHON_VERSION=2.7.3
export ROOT_VERSION=5.34.12

#GCC
source /afs/cern.ch/sw/lcg/contrib/gcc/$GCC_VERSION/$PLATFORM-gcc$GCC_VERSION_SHORT-opt/setup.sh
export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/contrib/gcc/$GCC_VERSION/$PLATFORM-gcc$GCC_VERSION_SHORT-opt/lib64:${LD_LIBRARY_PATH}

### Python
export PYTHONSYS="/afs/cern.ch/sw/lcg/external/Python/$PYTHON_VERSION/$PLATFORM-gcc$GCC_VERSION_SHORT-opt"
export PATH="$PYTHONSYS/bin:$PATH"
export LD_LIBRARY_PATH="$PYTHONSYS/lib:$LD_LIBRARY_PATH"

#ROOT
export ROOTSYS="/afs/cern.ch/sw/lcg/app/releases/ROOT/$ROOT_VERSION/$PLATFORM-gcc$GCC_VERSION_SHORT-opt/root"
# source $ROOTSYS/$ROOT_VERSION/$PLATFORM-gcc$GCC_VERSION_SHORT-opt/root/bin/thisroot.sh
export PATH="$ROOTSYS/bin:$PATH"
export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"
export PYTHONPATH="$ROOTSYS/lib:$PYTHONPATH"
export MANPATH="`dirname $ROOTSYS/man/man1`:$MANPATH"

#Some working directories etc...
export PYTHONPATH="/afs/cern.ch/user/g/gauzinge/scratch/CMSSW_6_2_0/src/analysis/python:$PYTHONPATH"
export PATH="~/bin:$PATH"

# actual script

echo 'Submitter script for CM Analysis to LXBATCH!'
echo 'Usage: ./'$0 'CBC(AorB) input_files(including_*)'

CHIPS='A B'
WORKINGDIR='/afs/cern.ch/user/g/gauzinge/scripts/analysis/'
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
		root -b -q -l $WORKINGDIR'src/cm_analysis.cc+("'$i'","'$1'")'
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
		root -b -q -l $WORKINGDIR'src/cm_analysis.cc+("'$i'","'$1'")'
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
echo 'Cleaning up root shared libraries!'
rm -rf *.d
rm -rf *.so
echo 'Done!'
