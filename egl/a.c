#include <GL/gl.h>
#include <EGL/egl.h>

#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH  256
#define HEIGHT 256

static const EGLint configAttribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
};

static const EGLint pbufferAttribs[] = {
    EGL_WIDTH , WIDTH ,
    EGL_HEIGHT, HEIGHT,
    EGL_NONE,
};

int main(int argc, char *argv[]) {

    // 1. Initialize EGL
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    EGLint major, minor;
    eglInitialize(display, &major, &minor);
    printf("version %d.%d\n", major, minor);

    // 2. Select an appropriate configuration
    EGLint numConfigs;
    EGLConfig config;

    eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);

    // 3. Create a surface
    EGLSurface surface = eglCreatePbufferSurface(display, config, pbufferAttribs);

    // 4. Bind the API
    eglBindAPI(EGL_OPENGL_API);

    // 5. Create a context and make it current
    EGLContext eglCtx = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);

    eglMakeCurrent(display, surface, surface, eglCtx);
    
    // from now on use your OpenGL context
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    eglSwapBuffers(display, surface);
    unsigned char pixels[WIDTH*HEIGHT*4] = {
        0xFF, 0x00, 0x00,
        0x00, 0xFF, 0x00,
        0x00, 0x00, 0xFF,
    }; 
    glReadPixels(0,0, WIDTH, HEIGHT,GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixels);
    stbi_write_png("a.png", WIDTH, HEIGHT, 4, pixels, WIDTH*4);

    // 6. Terminate EGL when finished
    eglTerminate(display);
    return 0;
}

