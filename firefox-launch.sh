#!/usr/bin/env bash

## This script is intended to be run from the desktop launcher 
## and passed the specific profile to launch as argument 1
## The whole point is to launch a fresh firefox if none are running,
## and launch any subsequent instances with the --no-remote flag
## Pretty good post about --no-remote here: https://www.brycevandyk.com/dissecting-firefoxs-no-remote-option/

## The files in /usr/share/applications/{firefox-personal.desktop,firefox-work.desktop}
## Have exec lines like: Exec=/usr/local/bin/firefox-launch.sh  Personal
  
SCRIPT_NAME=$(basename $(readlink -nf $0))
# We need to eliminate the name of this script
NON_SCRIPT_PIDS=$(ps -ef |grep --invert-match ${SCRIPT_NAME})

# check if firefox is already running
if echo $NON_SCRIPT_PIDS |grep -q "[f]irefox"; then
    echo "Firefox is already running. Starting with --no-remote"
    notify-send "Firefox is already running. Starting with --no-remote"
    $(env firefox --no-remote -P $1)
else
    echo "Starting firefox fresh"
    notify-send "Starting firefox"
   $(env firefox -P $1)
fi
