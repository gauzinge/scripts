#!/bin/bash 
# georg.auzinger@cern.ch
# script to run CBC analysis on cluster digi (after runningn stub finder) from any convenient location
# usage: copy to analysis directory and give files as input
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
cmsswdir='/afs/cern.ch/user/g/gauzinge/scratch/CMSSW_6_2_0/src'
homedir='/afs/cern.ch/user/g/gauzinge/'
echo 'Script for batched analysis of several runs!'
echo

if [[ $# -ne 0 ]];
	then
	echo 'goint to CMSSW src directory and setting the environment!'
	echo $cmsswdir
	cd $cmsswdir
	eval `scramv1 runtime -sh`
	echo
	counter=0;
	echo 'Analyzing files: '
	for i in $*
	do
		echo $i
	done
	
	for i in $*
	do
		echo 'Processing '$i':'
		# echo 'cmsRun CBC_Analysis.py' $homedir$i
		# cmsRun CBC_Analysis.py $homedir$i
		echo 'cmsRun CBC_Analysis.py' $i
		cmsRun CBC_Analysis.py $i
		((counter++));
	done
	cd
	echo 'Analyzed $counter files!'
else
	echo 'No data file provided!'
	echo 'Syntax ' $0 ' filename (including Wildchars!)';
fi