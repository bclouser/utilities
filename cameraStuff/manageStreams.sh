#!/usr/bin/env bash

CURRENT_DIR_NAME=streams01
NEXT_DIR_NAME=streams02
FILE_NAME=${CURRENT_DIR_NAME}/videoStream.avi

PID_FILE=lastKnownPid.txt
#CP_FILE_PATH=bclouser@192.168.0.192:./workspace/camStuff/incomingStreams
CP_FILE_PATH=/mnt/public/cameraStreamsRailBattery

LOG_FILE="streamsLog.log"
AVCONV_LOG_FILE="avconvStreams.log"


if [ "$EUID" -ne 0 ]
	then echo "Please run as root"
	exit
fi

# Initialize and start the log, clearing out anything that was in there
echo "$(date +%Y-%m-%d_%H-%M-%S): Ok, here we go " |tee $LOG_FILE
# TODO: we really should catch kill events and then take down all background proceses


# clean up any old versions of the streams.
rm -r ${CURRENT_DIR_NAME} ${NEXT_DIR_NAME}

mkdir ${CURRENT_DIR_NAME} ${NEXT_DIR_NAME}

function LaunchStream () {
	local STREAM_NUM=$1
	# Launch stream
	echo "============================================================" |tee -a $LOG_FILE
	echo "= Launching stream and saving to ${CURRENT_DIR_NAME}"        |tee -a $LOG_FILE
	echo "============================================================" |tee -a $LOG_FILE
	echo "$(date +%Y-%m-%d_%H-%M-%S) starting avconv" > $AVCONV_LOG_FILE # Because we need some space
	avconv -v warning -f video4linux2 -r 25 -s 1280x960 -i /dev/video0 ${CURRENT_DIR_NAME}/stream${STREAM_NUM}.avi &>> $AVCONV_LOG_FILE &

	PID=$!
	echo "$(date +%Y-%m-%d_%H-%M-%S): avconv PID is ${PID}" |tee -a $LOG_FILE
	echo $PID > lastKnownPid.txt
}

while [ 1 ]
do
	# Get the final name. Based on current day
	DATE=`date +%Y-%m-%d_%H-%M-%S`
	FINAL_FILE_NAME="stream_${DATE}.avi"

	STREAM_NUM=0
	# Start avconv program to capture stream
	LaunchStream $STREAM_NUM

	while [ 1 ]
	do
		# sleep for a small interval of time
		sleep 5

		# Calculate the number of seconds until its midnight
		current_epoch=$(date +%s)
		target_epoch=$(date -d '23:59:59' +%s)
		secondsTillMidnight=$(( $target_epoch - $current_epoch ))
		if [ $secondsTillMidnight -gt 7 ];then # 2s extra fluff in case we respawn process too soon
			if [ ! -d "/proc/$PID" ];then
				STREAM_NUM=$(($STREAM_NUM+1))
				echo "$(date +%Y-%m-%d_%H-%M-%S): Running process crashed. Restarting it" | tee -a $LOG_FILE
				# RE-Start avconv program to capture stream
				LaunchStream $STREAM_NUM
			fi
		else
			echo "$(date +%Y-%m-%d_%H-%M-%S): Leaving loop because its midnight" |tee -a $LOG_FILE
			# leave this loop, its midnight
			break
		fi
	done


	# So it should be midnight now, LET'S GET CRACKING!
	# kill the process
	kill $PID || {
		echo "" |tee -a $LOG_FILE
		echo "$(date +%Y-%m-%d_%H-%M-%S): Uh oh, failed to kill running process\n" | tee -a $LOG_FILE
		# Should we exit?
	}

	# Merge any and all avi files into final filename
	cd $CURRENT_DIR_NAME
	avimerge -i $(ls *.avi) -o $FINAL_FILE_NAME || {
		echo "$(date +%Y-%m-%d_%H-%M-%S): oof! Failed to merge avi files... I will let this slide for now" |tee -a $LOG_FILE
	}
	cd -

	echo "$(date +%Y-%m-%d_%H-%M-%S): Copying ${CURRENT_DIR_NAME} to ${CP_FILE_PATH}/${FINAL_FILE_NAME}" |tee -a $LOG_FILE
	# In the background send the file to some safe network store.
	cp -r ${CURRENT_DIR_NAME}/${FINAL_FILE_NAME} ${CP_FILE_PATH} &

	# we are assuming that the previous copy has finished. (24 hours should have passed)
	rm -r ${NEXT_DIR_NAME}/*
	sync

	# update filename so next time its different. Ping pong between two directories
	BEING_COPIED=${CURRENT_DIR_NAME}
	CURRENT_DIR_NAME=${NEXT_DIR_NAME}
	NEXT_DIR_NAME=${BEING_COPIED}


	echo "" |tee -a $LOG_FILE
	echo "" |tee -a $LOG_FILE

done

