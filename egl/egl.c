
#include <stdio.h>
#include <EGL/egl.h>
#include "egl.h"

static const EGLint configAttribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE , 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE  , 8,
    EGL_DEPTH_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
};

static const EGLint pbufferAttribs[] = {
    EGL_WIDTH , BUFFER_WIDTH ,
    EGL_HEIGHT, BUFFER_HEIGHT,
    EGL_NONE,
};

static EGLDisplay display;
static EGLint major, minor;
static EGLint numConfigs;
static EGLConfig config;
static EGLSurface surface;
static EGLContext context;

void egl_init() {

    // 1. Initialize EGL
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, &major, &minor);
    printf("version %d.%d\n", major, minor);
    // 2. Select an appropriate configuration
    eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);
    // 3. Create a surface
    surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
    // 4. Bind the API
    eglBindAPI(EGL_OPENGL_API);
    // 5. Create a context and make it current
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
    eglMakeCurrent(display, surface, surface, context);
}

void egl_swap() {
    eglSwapBuffers(display, surface);
}

void egl_close() {
    // 6. Terminate EGL when finished
    eglTerminate(display);
}
