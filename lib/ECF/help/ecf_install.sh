#!/usr/bin/env bash

# ECF has some dependencies:
# 	- SVN - for downloading the latest ECF version
#	- the C++ Boost library
#	- MPICH2 - for using multiple processor cores

SVN="subversion"
BOOSTLIB="libboost-dev"
LIBTOOL="libtool"
MPICH2="mpich2"
AUTORECONF="dh-autoreconf"

NCORES=$(cat /proc/cpuinfo |grep -c "model name")
CURR_DIR=$(pwd)

printf "%b\n"
echo "-----------------"
echo "-----------------" 
echo "SOME INFO ABOUT YOUR SYSTEM - WHEN REPORTING A PROBLEM, INCLUDE THIS INFO"
echo "-----------------"
uname -a
cat /etc/*-release
echo "-----------------"
echo "-----------------"
printf "%b\n" 
echo $NCORES 
echo "By default ECF installs commands under /usr/local/bin, include files under /usr/local/include, etc. If other location is preffered (e.g. /home/user/ECF/), read the INSTALL file and exit this script NOW"
printf "%b\n"

# -----------------
# |UBUNTU SPECIFIC|
# -----------------
echo "ECF needs Subversion, C++ Boost library, libtool and autoreconf. Install now ("yes") or skip if they exist? (if unsure enter "yes") [y/n]:"
read DEPS
if [[ $DEPS = y* ]]; then
	sudo apt-get install $SVN $BOOSTLIB $LIBTOOL $AUTORECONF
fi
echo "Downloading latest ECF version to 'trunk/'."
svn co svn://ecf.zemris.fer.hr/ecf/trunk

# mpi.h and other not seen, a quick&dirty solution
echo "Need password for 'ln -s /usr/include/mpich2/* /usr/local/include'."
sudo ln -s /usr/include/mpich2/* /usr/local/include

#the compiler complains that strlen() is unresolved, so we need to patch 2 files
#printf "%b\n"
#echo "Applying Ubuntu specific patches..."
#echo "#include \"string.h\"" | tee -a temp1 temp2
#cat $CURR_DIR/trunk/ECF/Communicator.cpp >> temp1
#mv temp1 $CURR_DIR/trunk/ECF/Communicator.cpp
#cat $CURR_DIR/trunk/ECF/Population.cpp >> temp2
#mv temp2 $CURR_DIR/trunk/ECF/Population.cpp


cd $CURR_DIR/trunk/

printf "%b\n"
echo "Number of CPU cores detected:" $NCORES
echo "-------------------------------"
printf "%b\n"
if [ "$NCORES" != "1" ]; then
	echo "For using more than one CPU core ECF needs an MPI-like system. Do you want to compile ECF now with MPI support ("n" for later)? [y/n]:"
	read MPINOW

	if [[ $MPINOW = y* ]]; then

		echo "Do you already have a working MPI implementation? (if unsure enter "no") [y/n]:"
		read MPIINST
	
		if [[ $MPIINST = y* ]]; then
			echo "Path to your MPI installation (e.g. /usr/include/mpich2/):"
			read MPIPATH
		
			autoreconf -fi
			./configure --with-mpi=$MPIPATH
		else
			echo "Will install MPICH2:"
			sudo apt-get install $MPICH2
			MPIPATH="/usr/include/mpich2"
		
			autoreconf -fi		
			./configure --with-mpi=$MPIPATH
		fi
	else
		echo "Compiling without MPI support."
		autoreconf -fi
		./configure
	fi
else
	echo "Only one CPU core is detected, ECFs parallel feature will be disabled."
	autoreconf -fi
	./configure
fi

	make -j $NCORES
	make -j $NCORES check
	sudo make install

echo
echo "Script complete, try running the examples in /trunk/examples/ or see the Tutorial in /trunk/help/"

exit 1
