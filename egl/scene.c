#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>

#include "scene.h"

float rnd() { return (float)rand()/(float)(RAND_MAX); }
#define INFO(attr) fprintf(stderr,"%*s: %s\n",32, #attr, glGetString(attr))
static void context_info() {
    GLint major, minor, cnt;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    fprintf(stderr, "OpenGL %d.%d\n", major, minor);
    INFO(GL_VENDOR);
    INFO(GL_RENDERER);
    INFO(GL_VERSION);
    INFO(GL_SHADING_LANGUAGE_VERSION);

    // INFO(GL_EXTENSIONS);
}

void scene_init() { context_info(); }

void scene_draw(int frame) {
  // from now on use your OpenGL context
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glBegin(GL_TRIANGLE_STRIP);
  for (int i = 0; i < 10; i++) {
    glColor4f(rnd(), rnd(), rnd(), rnd());
    glVertex3f((0.5 - rnd()) * 2.0, (0.5 - rnd()) * 2.0, 0);
    glColor4f(rnd(), rnd(), rnd(), rnd());
    glVertex3f((0.5 - rnd()) * 2.0, (0.5 - rnd()) * 2.0, 0);
    glColor4f(rnd(), rnd(), rnd(), rnd());
    glVertex3f((0.5 - rnd()) * 2.0, (0.5 - rnd()) * 2.0, 0);
  }
  glEnd();
  glFlush();
}

void scene_close() {}
 
