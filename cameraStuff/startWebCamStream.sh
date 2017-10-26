#!/usr/bin/env bash

cvlc -vvv v4l2:///dev/video0 --http-port=25565 --sout '#transcode{vcodec=FLV1,acodec=mpga,vb=512,ab=64, samplerate=44100}:standard{access=http{mime=video/x-flv},mux=ffmpeg{mux=flv},dst=:8081/stream.flv}'
