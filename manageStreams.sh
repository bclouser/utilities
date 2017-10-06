#!/usr/bin/env bash

CURRENT_FILE_NAME=videoStream1.avi
NEXT_FILE_NAME=videoStream2.avi

#CP_FILE_PATH=bclouser@192.168.0.192:./workspace/camStuff/incomingStreams
CP_FILE_PATH=/mnt/public/cameraStreamsRailBattery



if [ "$EUID" -ne 0 ]
	then echo "Please run as root"
	exit
fi

# clean up any old versions of the streams.
rm ${CURRENT_FILE_NAME} ${NEXT_FILE_NAME}

while [ 1 ]
do
	# Get the final name. Based on current day
	DATE=`date +%Y-%m-%d_%H-%M-%S`
	FINAL_FILE_NAME="stream_${DATE}.avi"

	# Launch stream
	echo "============================================================"
	echo "= Launching stream and saving to ${CURRENT_FILE_NAME}"
	echo "============================================================"
	avconv -f video4linux2 -r 25 -s 1280x960 -i /dev/video0 ${CURRENT_FILE_NAME} &

	PID=$!
	echo "PID is ${PID}"

	# just make sure we dont loop around in the same second
	sleep 1

	# Calculate the number of seconds until its midnight
	current_epoch=$(date +%s)
	target_epoch=$(date -d '23:59:59' +%s)
	sleep_seconds=$(( $target_epoch - $current_epoch ))

	echo "sleep seconds ${sleep_seconds}"

	sleep $sleep_seconds

	# So it should be midnight now, LET'S GET CRACKING!
	# kill the process and start a new one.
	kill $PID || {
		echo "Uh oh, failed to kill running process\n"
		# Should we exit?
	}

	# In the background send the file to some safe network store. (shared keys earlier, no password necessary)
	#scp ${CURRENT_FILE_NAME} ${SCP_FILE_PATH}/${FINAL_FILE_NAME} &
	cp ${CURRENT_FILE_NAME} ${CP_FILE_PATH}/${FINAL_FILE_NAME} &

	# we are assuming that the copy has finished.
	rm ${NEXT_FILE_NAME}
	sync

	# update filename so next time its different. Ping pong between two
	BEING_COPIED=${CURRENT_FILE_NAME}
	CURRENT_FILE_NAME=${NEXT_FILE_NAME}
	NEXT_FILE_NAME=${BEING_COPIED}


	echo ""
	echo ""

done




