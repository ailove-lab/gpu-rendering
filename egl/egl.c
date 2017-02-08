
#include <stdio.h>
#include <unistd.h>

#include <GL/gl.h>
#include <EGL/egl.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

static void check_err();

void egl_init() {

    // 1. Initialize EGL
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, &major, &minor);
    check_err();
    printf("version %d.%d\n", major, minor);
    // 2. Select an appropriate configuration
    eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);
    check_err();
    // 3. Create a surface
    surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
    check_err();
    // 4. Bind the API
    eglBindAPI(EGL_OPENGL_API);
    check_err();
    // 5. Create a context and make it current
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
    check_err();
    eglMakeCurrent(display, surface, surface, context);
    check_err();
}

void egl_swap() {
    eglSwapBuffers(display, surface);
}

unsigned char pixels[BUFFER_WIDTH*BUFFER_HEIGHT*4];
void egl_save(char const* filename) {
    glReadPixels(0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,
                 GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixels);
    stbi_write_png(filename, BUFFER_WIDTH, BUFFER_HEIGHT, 4, pixels, BUFFER_WIDTH*4);
}

static FILE* out;
void egl_init_stream() {
   out = fdopen(dup(fileno(stdout)), "wb");
}
void egl_write_stream() {
    glReadPixels(0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,
                 GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixels);

    fwrite(pixels, BUFFER_WIDTH*BUFFER_HEIGHT*4,1,out);
}
void egl_close_stream() {
    fclose(out);
}

void egl_close() {
    // 6. Terminate EGL when finished
    eglTerminate(display);
}

static void check_err() {
   EGLint err = eglGetError();
   if(err!=0x3000) {
       fprintf(stderr, "Error %h\n", err);
   } else {
       printf("OK\n");
   }
}
