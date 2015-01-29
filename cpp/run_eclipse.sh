#!/bin/bash

# declare ECLIPSE_LOCATION variable
#to do declare ACE_ROOT_LOCATION variable




export ACE_ROOT="/home/strogos/ACE/ACE_wrappers"
export LD_LIBRARY_PATH=/home/strogos/ACE/ACE_wrappers/lib:$LD_LIBRARY_PATH

cd /home/strogos/eclipse
./eclipse&


