#!/bin/bash
#build python for host
#rename binaries
#make distclean
#patch


function printLogLocation {
	echo "see the logfile located: ${PYTHON_CC_LOGFILE}"
}

function usage {
	echo " "
	echo "usage:  <python tar file>.tar   <output directory>  "
	echo "The python tar file(for 2.7.3) is located: https://www.python.org/ftp/python/2.7.3/Python-2.7.3.tgz "
	echo " "
}

#check the number of args
if [ $# -ne 2 ]
then
	usage
	exit
fi

#check if the user is asking for help
if [ $1 == "-h" ] || [ $1 == "-H" ] || [ $1 == "-help" ] || [ $1 == "--help" ]
then
	usage
	exit
fi

PYTHON_CC_LOGFILE=${PWD}/cc_python_build.log
PYTHON_TAR_FILE=${1}
#remove leading whitespaces
PYTHON_TAR_FILE="${PYTHON_TAR_FILE##*()}"
#remove trailing whitespaces
PYTHON_TAR_FILE="${PYTHON_TAR_FILE%%*()}"


#make sure that argument 1 is the python tar file
if [ ${PYTHON_TAR_FILE: -4} != ".tar"  ] && [ ${PYTHON_TAR_FILE: -4} != ".tgz" ] \
        && [ ${PYTHON_TAR_FILE: -7} != ".tar.gz" ]
then
    echo "argument 1 should be the python tar file"
    printLogLocation
    exit
fi

#${<var name>%/} This strips off the trailing / from input paths if it exists.
OUTPUT_DIRECTORY=${2%/}
#makes the output directory an absolute path
OUTPUT_DIRECTORY=${PWD}/${OUTPUT_DIRECTORY}

#remove any beginning ./
if [[ $PYTHON_TAR_FILE == ./* ]]
then
	PYTHON_TAR_FILE=${PYTHON_TAR_FILE##*/}
fi
echo "after removing leading slash: $PYTHON_TAR_FILE"

#set python directory by removing file extension from tar file
PYTHON_DIRECTORY=${PYTHON_TAR_FILE%.*}

echo "PYTHON_DIRECTORY: ${PYTHON_DIRECTORY}"
echo "OUTPUT_DIRECTORY: ${OUTPUT_DIRECTORY}"

#make the PYTHON_DIRECTORY an absolute path.
PYTHON_DIRECTORY=${PWD}/${PYTHON_DIRECTORY}

echo "absolute python directory $PYTHON_DIRECTORY "


#First make sure we can get python and correct  patch for the python version we are cross compiling.
#wget https://www.python.org/ftp/python/2.7.3/Python-2.7.3.tgz || {
#        echo "failed to download the python tar file... Stopping."
#}
echo " "
echo "Fetch python cross compile patch"
wget http://randomsplat.com/wp-content/uploads/2012/10/Python-2.7.3-xcompile.patch || {
	echo "failed to download the crossCompile patch... Stopping."
	printLogLocation
	exit
}

echo "Extracting $PYTHON_TAR_FILE into $PYTHON_DIRECTORY"
tar -xvf $PYTHON_TAR_FILE > ${PYTHON_CC_LOGFILE} 2>&1 #"&>>" doesn't work on bash < 4

cd $PYTHON_DIRECTORY
echo " "
echo "configuring for this build machine"
#Build python first on the host computer. build it as if it will run on the host system
./configure >> ${PYTHON_CC_LOGFILE} 2>&1 || {
	echo "failed while performing build machine configure... stopping"
	printLogLocation
	exit
}
echo " "
echo "Building python for this build machine"
make python Parser/pgen >> ${PYTHON_CC_LOGFILE} 2>&1 || {
	echo "failed while initial build of python for the build machine... stopping"
	printLogLocation	
	exit
}

#rename the files we need for our subsequent builds
echo " "
echo "renaming important binaries to be used while cross compiling"
mv python hostpython
mv Parser/pgen Parser/hostpgen

#clean up build directory, remove stuff from the previous build
echo " "
echo "performing \"make distclean\""
make distclean >> ${PYTHON_CC_LOGFILE} 2>&1

#patch the files
echo " "
echo "Applying Patches:"
patch -p1 < ../Python-2.7.3-xcompile.patch || {
	echo "hmmm, patch failed... stopping"
	printLogLocation
	exit
}


#cross compile variables
HOST_PYTHON="./hostpython"
HOST_PGEN="./Parser/hostpgen"
BLD_SHARED="${CROSS_COMPILE}gcc -shared"
CC_TARGET=yes
#export HOSTARCH="i386-linux-gnu"
#export BUILDARCH="arm-linux-gnu"
HOST_ARCH="arm-linux"
BUILD_ARCH="arm-linux-gnu"

#cross compilers used for python compilation
#export CC=${CROSS_COMPILE}gcc
#export CXX=${CROSS_COMPILE}g++ 
#export AR=${CROSS_COMPILE}ar
#export RANLIB=${CROSS_COMPILE}ranlib 

cd $PYTHON_DIRECTORY
echo " "
echo "configuring for host:${HOST_ARCH} , build: ${BUILD_ARCH}"
CC=${CROSS_COMPILE}gcc CXX=${CROSS_COMPILE}g++ AR=${CROSS_COMPILE}ar RANLIB=${CROSS_COMPILE}ranlib \
		./configure --host=${HOST_ARCH} --build=${BUILD_ARCH} --prefix=${OUTPUT_DIRECTORY} >> ${PYTHON_CC_LOGFILE} 2>&1 || {
   	echo "well. configure for the target failed... Stopping"
   	printLogLocation
   	exit
}

echo " "
echo "executing: make (building for target)"

make HOSTPYTHON=$HOST_PYTHON BLDSHARED="${BLD_SHARED}" CROSS_COMPILE=${CROSS_COMPILE} \
	HOSTPGEN=$HOST_PGEN CROSS_COMPILE_TARGET=yes HOSTARCH=$HOST_ARCH BUILDARCH=$BUILD_ARCH >> ${PYTHON_CC_LOGFILE} 2>&1 || {
	echo " build failed"
	printLogLocation
	exit
}
echo " "
echo "executing: make install"
make install HOSTPYTHON=$HOST_PYTHON BLDSHARED="{$BLD_SHARED}" CROSS_COMPILE=${CROSS_COMPILE} \
			 CROSS_COMPILE_TARGET=yes prefix=$OUTPUT_DIRECTORY/_install >> ${PYTHON_CC_LOGFILE} 2>&1 || {
	echo "make install failed"
	printLogLocation	
	exit
}
echo " "
echo "python was successfully cross compiled. Copy the directory inside $OUTPUT_DIRECTORY to the target"



