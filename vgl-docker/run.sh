#!/bin/bash
docker run \
       --privileged \
       --rm \
       -ti \
       -e DISPLAY \
       -v /tmp/.X11-unix:/tmp/.X11-unix:ro \
       -u docker \
       yantis/virtualgl /bin/bash
