#!/bin/bash

# Created by, Jeremy Schwartz on 10/8/17
#
# Recursivly extract the dependencies for a c/c++ file.
# To be called from a makefile to determine dependencies.

# First argument should be the file to be scanned.
FILE="$1"

# Absolute directory of this script.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# For some reason the makefile checks "makefile.cpp" twice.
# Until the cause is found, this is a quick work around.
if [[ "$FILE" == "makefile.cpp" ]]; then
    exit
fi

# Extracts the directory from the input argument.
# Used so that the recursive call will be checking relative
# to the scanning file.
#
# Example:
#   scan ./dir/a.cpp
#   - find #include "a.h"
#   - a.h is located in ./dir/ and not in ./
#   The macro extraction will extract 'a.h' so we prepend
#   the directory of the scanning file './dir/' to ensure
#   that we are looking at the correct file.
if [[ "$FILE" == *"/"* ]]; then
    FDIR="${FILE%/*}/"
else
    FDIR=""
fi

# Variable to hold the dependencies.
INCLUDE_FILES=""

# Look though each line in the file for lines that start
# with '#include "'. If one is found then extract the contents
# of the #include and add them to the INCLUDE_FILES string.
while read p
do
    if [[ "$p" == \#include\ \"* ]]; then
        str=${p:10}
        str=${str%\"}
        # prepend the relative direcotry and add file
        INCLUDE_FILES+="${FDIR}${str} "
    fi
done < $FILE # Feed the file into the read command.

# Recursivly look for more dependencies.
for f in $INCLUDE_FILES
do
    INCLUDE_FILES+="$($SCRIPT_DIR/dep.sh $f)"
done

# Echo the dependencies to stdout or the calling script.
# This is the output of this script.
echo "${INCLUDE_FILES[@]}"
