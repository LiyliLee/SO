#!/bin/bash

MPOINT="./mount-point"

rm -R -f temp
mkdir temp

echo 'Copiando fuseLib.c y myFS.h a SF y ./temp'
cp ./src/fuseLib.c $MPOINT
cp ./src/fuseLib.c ./temp
cp ./src/myFS.c $MPOINT
cp ./src/myFS.c ./temp

diff ./src/fuseLib.c ~

read -p "Press enter..."

echo "Creating file 2"
echo 'This is file 2' > $MPOINT/file2.txt
ls $MPOINT -la
read -p "Press enter..."







######################

# #!/bin/bash

# DIRECTORY_S="./src" 
# DIRECTORY_T="./temp" 
# DIRECTORY_M="./mount-point" 

# FILE1="myFS.h"
# FILE2="fuseLib.c"
# FILE3="MyFileSystem.c"

# rm -rf $DIRECTORY_T 
# mkdir $DIRECTORY_T 

# # a)

# cp $DIRECTORY_S/$FILE1 $DIRECTORY_T/$FILE1
# cp $DIRECTORY_S/$FILE1 $DIRECTORY_M/$FILE1

# cp $DIRECTORY_S/$FILE2 $DIRECTORY_T/$FILE2	
# cp $DIRECTORY_S/$FILE2 $DIRECTORY_M/$FILE2

# # b)

# ./my-fsck-static-64 virtual-disk
# read -p "Press enter..."

# diff $DIRECTORY_M/$FILE1 $DIRECTORY_T/$FILE1

# if [ $? = 0 ]; then 
# 	echo "The file 1 is the same in the mount and temp directory"

# 	diff $DIRECTORY_M/$FILE2 $DIRECTORY_T/$FILE2
# 	if [ $? = 0 ]; then
# 		echo "The file 2 is the same in the mount and temp directory"
# 	else
# 		echo "The file 1 is different"
# 	fi
# else 
# 	echo "The file 1 is different"
# fi

# read -p "Press enter..."

# truncate --size=-4096 $DIRECTORY_T/$FILE1
# truncate --size=-4096 $DIRECTORY_M/$FILE1

# echo "File 1 truncated"
# read -p "Press enter..."
# # c)

# ./my-fsck-static-64 virtual-disk
# read -p "Press enter..."

# diff $DIRECTORY_M/$FILE1 $DIRECTORY_S/$FILE1

# if [ $? = 0 ]; then 
# 	echo "The file 1 is the same in the mount and source directory"
# else 
# 	echo "The file 1 is different in the mount and source directory"
# fi

# # d)

# cp $DIRECTORY_S/$FILE3 $DIRECTORY_M/$FILE3
# read -p "Press enter..."

# # e)

# ./my-fsck-static-64 virtual-disk
# read -p "Press enter..."

# diff $DIRECTORY_S/$FILE3 $DIRECTORY_M/$FILE3

# if [ $? = 0 ]; then 
# 	echo "The file 3 is the same in the mount and source directory"
# else 
# 	echo "The file 3 is different in the mount and source directory"
# fi

# read -p "Press enter..."
# # f)

# truncate --size=+4096 $DIRECTORY_T/$FILE2
# truncate --size=+4096 $DIRECTORY_M/$FILE2

# echo "File 2 truncated"
# read -p "Press enter..."

# # g)
# ./my-fsck-static-64 virtual-disk
# read -p "Press enter..."

# diff $DIRECTORY_S/$FILE2 $DIRECTORY_M/$FILE2

# if [ $? = 0 ]; then 
# 	echo "The file 2 is the same in the mount and source directory"
# else 
# 	echo "The file 2 is different in the mount and source directory"
# fi

# read -p "Press enter..."
# diff $DIRECTORY_S/$FILE2 $DIRECTORY_T/$FILE2

# if [ $? = 0 ]; then 
# 	echo "The file 2 is the same in the source and temp directory"
# else
# 	echo "The file 2 is different in the source and temp directory"
# fi
 