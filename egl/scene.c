#include <GL/gl.h>
#include <stdlib.h>

#include "scene.h"

float rnd() { return (float)rand()/(float)(RAND_MAX); }

void scene_draw(int frame) {
    // from now on use your OpenGL context
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLE_STRIP);
    for(int i=0; i< 10; i++) {
        glColor4f(rnd(), rnd(), rnd(), rnd());
        glVertex3f((0.5-rnd())*2.0, (0.5-rnd())*2.0, 0);
        glColor4f(rnd(), rnd(), rnd(), rnd());
        glVertex3f((0.5-rnd())*2.0, (0.5-rnd())*2.0, 0);
        glColor4f(rnd(), rnd(), rnd(), rnd());
        glVertex3f((0.5-rnd())*2.0, (0.5-rnd())*2.0, 0);
    }
    glEnd();
    glFlush();
}

