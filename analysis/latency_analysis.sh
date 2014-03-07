#! /bin/bash

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

WORKINGDIR='/afs/cern.ch/user/g/gauzinge/scripts/analysis/'

echo 'Submitting Latency Analysis!'
echo 'root -b -q -l src/latency_analysis.cc+'
root -b -q -l $WORKINGDIR'src/latency_analysis.cc+'

echo 'Cleaning up root shared libraries!'

rm -rf $WORKINGDIR/src/latency_analysis_cc.d
rm -rf $WORKINGDIR/src/latency_analysis_cc.so

echo 'Done!'