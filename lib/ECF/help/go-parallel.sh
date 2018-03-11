#!/usr/bin/env bash

#when the user hits CTRL+c do:
quit()
{
printf "%b\n"
echo "Shuting down mpd."
mpdallexit
sleep 1;
printf "%b\n"
echo "Finished, have a nice day."
exit $?
}
trap quit 2


NCORESALL=$(cat /proc/cpuinfo |grep -c "model name")
printf "%b\n"

# first run and a hint
if [ -f $HOME/.mpd.conf ]; then 
	echo "------------------------------------"	
	echo "This script supports one multicore machine. If you want to use multiple machines please read a tutorial on running MPICH2 applications."
	echo "------------------------------------"
else
	echo "You want to run parallel jobs? Just a second. The MPI framework requires a special file - .mpd.conf in your home directory that contains a secret phrase in case you will later add more machines and create a cluster. Just enter your secret word, we will store it in $HOME/.mpd.conf."
read PHRASE
echo $PHRASE >> $HOME/.mpd.conf
chmod 600 $HOME/.mpd.conf
echo "Finished setting $HOME/.mpd.conf."
printf "%b\n"
fi
	
#finding all executable files in current directory and removing ourselfs from the result
EXE=$(ls|xargs file|grep executable| cut -d":" -f1|grep -v go-parallel.sh)

if [ "$EXE" != "" ]; then
	printf "%b\n"	
	echo "If needed enter some arguments or just hit ENTER."
	echo $EXE 	
	read PARAM
		
	echo $EXE $PARAM
else 
	printf "%b\n"	
	echo "Did not find any executable files. Copy this script to your working directory or check for compiler errors."
exit 1
fi

printf "%b\n"
echo "How many cores do you want to use on this machine? (number/all)"
read NCORES

#echo "Running in 3"
#sleep 1;
echo "Running in 2"
#starting mpd, it takes some time...
mpd --ncpus=$NCORESALL &
sleep 1;
echo "           1"
sleep 1;

if [ "$NCORES" != "all" ]; then
	echo "Using " $NCORES " cores"	
	mpiexec -n $NCORES ./$EXE $PARAM
else
	echo "Using all " $NCORES "cores."
	mpiexec -n $NCORESALL ./$EXE $PARAM
fi

#if finished, close all needed programs
mpdallexit
exit 1
