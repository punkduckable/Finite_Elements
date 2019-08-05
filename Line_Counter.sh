#!/bin/bash

# Initialize total to 0
total=0


################################################################################
# Count lines in all source files (.cc and .h)

# First, get all the .cc and .h files as a list.
FILE_LIST=$(find {./source,./test,} | grep -E  "((.h$)|(.cc$))")

# Now loop through tie files, adding the length of each one to total
for File in $FILE_LIST
do
  count=$(grep -c ^ $File)
  let total=total+count
done


################################################################################
# Add in makefile
let total=total+$(grep -c ^ ./Makefile)


################################################################################
# Report total.
echo $total
