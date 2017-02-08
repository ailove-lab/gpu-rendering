
#include <stdio.h>

#include "egl.h"
#include "scene.h"

int main(int argc, char *argv[]) {

    egl_init();
    egl_init_stream();
    char name[32];
    for(int i=0; i<30; i++) {
        scene_draw(i);
        egl_swap();
        sprintf(name, "test_%d.png", i);
        egl_write_stream();
    }
    egl_close_stream();
    egl_close();

    return 0;
}

