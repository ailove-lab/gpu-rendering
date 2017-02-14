#!/bin/bash
./test02 | \
    ffmpeg \
        -f rawvideo \
        -pix_fmt rgb32 \
        -s:v 1280x720\
        -r 30 \
        -i - \
        -c:v libx264 \
        -y \
        test02.mp4
ffplay test02.mp4
# 
