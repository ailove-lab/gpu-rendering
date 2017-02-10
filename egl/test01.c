
#include <stdio.h>

#include "egl.h"
#include "scene.h"

int main(int argc, char *argv[]) {

    int err = egl_init();
    if(err<0) return err;

    scene_init();

    char name[32];
    for(int i=0; i<5; i++) {
        scene_draw(i); 
        egl_swap();
        sprintf(name, "test_%d.png", i);
        egl_save(name);
    }

    egl_close();

    return 0;
}

