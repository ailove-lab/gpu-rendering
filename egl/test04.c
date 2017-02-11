#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl31.h>
#include <assert.h>
#include <fcntl.h>
#include <gbm.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define FALSE 0
#define TRUE 1

#define WIDTH  256
#define HEIGHT 256

int32_t fd;
struct gbm_device *gbm;
EGLDisplay egl_dpy;
EGLContext core_ctx;

GLuint program;

void egl_init();
void gl_info();
GLuint gl_load_shader( GLenum type, const char *shaderSrc );
int gl_init();
void gl_do();
void gl_clear();
void gl_write(char const* filename);
void egl_clear();

int32_t
main (int32_t argc, char* argv[]) {

    egl_init();
    gl_info();
    gl_init();

    gl_do();
    gl_write("test04.png");
    gl_clear();
    egl_clear();

    return 0;
}

void egl_init() {

  bool res;

  fd = open("/dev/dri/renderD128", O_RDWR);
  assert(fd > 0);

  gbm = gbm_create_device(fd);
  assert(gbm != NULL);

  /* setup EGL from the GBM device */
  egl_dpy = eglGetPlatformDisplay(EGL_PLATFORM_GBM_MESA, gbm, NULL);
  assert(egl_dpy != NULL);

  res = eglInitialize(egl_dpy, NULL, NULL);
  assert(res);

  const char *egl_extension_st = eglQueryString(egl_dpy, EGL_EXTENSIONS);
  assert(strstr(egl_extension_st, "EGL_KHR_create_context") != NULL);
  assert(strstr(egl_extension_st, "EGL_KHR_surfaceless_context") != NULL);

  static const EGLint config_attribs[] = {
      EGL_RENDERABLE_TYPE,  EGL_OPENGL_ES3_BIT_KHR,
      EGL_NONE
  };
  EGLConfig cfg;
  EGLint count;

  res = eglChooseConfig(egl_dpy, config_attribs, &cfg, 1, &count);
  assert(res);

  static const EGLint pbuffer_attribs[] = {
      EGL_WIDTH , WIDTH ,
      EGL_HEIGHT, HEIGHT,
      EGL_NONE,
  };
  res = eglCreatePbufferSurface(egl_dpy, cfg, pbuffer_attribs);

  res = eglBindAPI(EGL_OPENGL_ES_API);
  assert(res);
  
  static const EGLint attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 3,
      EGL_NONE
  };
  core_ctx = eglCreateContext(egl_dpy, cfg, EGL_NO_CONTEXT, attribs);
  assert(core_ctx != EGL_NO_CONTEXT);

  res = eglMakeCurrent(egl_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, core_ctx);
  assert(res);
}

void gl_info() {

   /* print some compute limits (not strictly necessary) */
   GLint work_group_count[3] = {0};
   for (unsigned i = 0; i < 3; i++)
      glGetIntegeri_v (GL_MAX_COMPUTE_WORK_GROUP_COUNT,
                       i, &work_group_count[i]);

   printf ("GL_MAX_COMPUTE_WORK_GROUP_COUNT: %d, %d, %d\n",
           work_group_count[0],
           work_group_count[1],
           work_group_count[2]);

   GLint work_group_size[3] = {0};
   for (unsigned i = 0; i < 3; i++)
      glGetIntegeri_v (GL_MAX_COMPUTE_WORK_GROUP_SIZE, i, &work_group_size[i]);
   printf ("GL_MAX_COMPUTE_WORK_GROUP_SIZE: %d, %d, %d\n",
           work_group_size[0],
           work_group_size[1],
           work_group_size[2]);

   GLint max_invocations;
   glGetIntegerv (GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &max_invocations);
   printf ("GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS: %d\n", max_invocations);

   GLint mem_size;
   glGetIntegerv (GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &mem_size);
   printf ("GL_MAX_COMPUTE_SHARED_MEMORY_SIZE: %d\n", mem_size);
}

// Create a shader object, load the shader source, and
// compile the shader.
GLuint gl_load_shader( GLenum type, const char *shaderSrc ) {

    GLuint shader;
    GLint compiled;
    // Create the shader object
    shader = glCreateShader ( type );
    if ( shader == 0 ) {
        return 0;
    }
    // Load the shader source
    glShaderSource ( shader, 1, &shaderSrc, NULL );
    // Compile the shader
    glCompileShader ( shader );
    // Check the compile status
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

    if ( !compiled ) {
        GLint infoLen = 0;
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        if ( infoLen > 1 ) {
            char *infoLog = malloc ( sizeof ( char ) * infoLen );

            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            printf( "Error compiling shader:\n%s\n", infoLog );
            free ( infoLog );
        }
        glDeleteShader ( shader );
        return 0;
    }
    return shader;
}

int
gl_init() {

   char vShaderStr[] =
      "#version 300 es                          \n"
      "layout(location = 0) in vec4 vPosition;  \n"
      "void main()                              \n"
      "{                                        \n"
      "   gl_Position = vPosition;              \n"
      "}                                        \n";

   char fShaderStr[] =
      "#version 300 es                              \n"
      "precision mediump float;                     \n"
      "out vec4 fragColor;                          \n"
      "void main()                                  \n"
      "{                                            \n"
      "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
      "}                                            \n";

   GLuint vertexShader;
   GLuint fragmentShader;
   GLint linked;

   // Load the vertex/fragment shaders
   vertexShader   = gl_load_shader ( GL_VERTEX_SHADER, vShaderStr );
   fragmentShader = gl_load_shader ( GL_FRAGMENT_SHADER, fShaderStr );

   // Create the program object
   program = glCreateProgram ( );

   if ( program == 0 ) {
      return 0;
   }

   glAttachShader ( program, vertexShader );
   glAttachShader ( program, fragmentShader );

   // Link the program
   glLinkProgram ( program );

   // Check the link status
   glGetProgramiv ( program, GL_LINK_STATUS, &linked );

   if ( !linked ) {
      GLint infoLen = 0;
      glGetProgramiv ( program, GL_INFO_LOG_LENGTH, &infoLen );
      if ( infoLen > 1 ) {
         char *infoLog = malloc ( sizeof ( char ) * infoLen );
         glGetProgramInfoLog ( program, infoLen, NULL, infoLog );
         printf( "Error linking program:\n%s\n", infoLog );
         free ( infoLog );
      }
      glDeleteProgram ( program );
      return FALSE;
   }

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

void
gl_do() {
    GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                            -0.5f, -0.5f, 0.0f,
                             0.5f, -0.5f, 0.0f
    };
    // Set the viewport
    glViewport ( 0, 0, WIDTH, HEIGHT );
    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );
    // Use the program object
    glUseProgram( program );
    // Load the vertex data
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    glEnableVertexAttribArray( 0 );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    
}

unsigned char pixels[WIDTH*HEIGHT*4];
void gl_write(char const* filename) {
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_INT_24_8, pixels);
    stbi_write_png(filename, WIDTH, HEIGHT, 4, pixels, WIDTH*4);
}

void
gl_clear(){
    glDeleteProgram(program);
}

void
egl_clear() {
    eglDestroyContext(egl_dpy, core_ctx);
    eglTerminate(egl_dpy);
    gbm_device_destroy(gbm);
    close(fd);
}
