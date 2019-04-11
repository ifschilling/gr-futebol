#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/ifschilling/gr-futebol/lib
export PATH=/home/ifschilling/gr-futebol/build/lib:$PATH
export LD_LIBRARY_PATH=/home/ifschilling/gr-futebol/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-futebol 
