#!/bin/bash
../build/egl/test02 | \
    ffmpeg \
        -f rawvideo \
        -pix_fmt rgb32 \
        -s:v 1280x720\
        -r 60 \
        -i - \
        -threads 8 \
        -c:v libx264 \
        -f flv \
        rtmp:///live/myStream.sdp
