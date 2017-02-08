#include <GL/gl.h>
#include <stdio.h>

#include "egl.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void draw();
void save_image();

int main(int argc, char *argv[]) {

    egl_init();

    draw();
    egl_swap();
    save_image();

    egl_close();

    return 0;
}

void draw() {
    // from now on use your OpenGL context
    glClearColor(1.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void save_image() {
    unsigned char pixels[BUFFER_WIDTH*BUFFER_HEIGHT*4];
    glReadPixels(0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixels);
    stbi_write_png("a.png", BUFFER_WIDTH, BUFFER_HEIGHT, 4, pixels, BUFFER_WIDTH*4);
}
