#!/bin/bash
./build/test02 | \
    ffmpeg \
        -f rawvideo \
        -pix_fmt rgb32 \
        -s:v 512x512 \
        -r 10 \
        -i - \
        -c:v libx264 \
        -y \
        test02.mp4
