#!/usr/bin/env bash

cvlc -vvv v4l2:///dev/video0 --sout '#transcode{vcodec=mp4v,vb=800}:file{dst=cameraVid.mp4}'

