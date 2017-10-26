#!/usr/bin/env bash

#ffmpeg -pixel_format yuv420p -video_size 640x480 -framerate 30 -i myimage.yuv imageOut%03d.png


#ffmpeg -i myimage.yuv -video_size 640x480 -framerate 30 -s 640x480 imageOut.png

ffmpeg -pix_fmt yuv422p -s 320x240 -i myimage.yuv out.jpg


