#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GLES3/gl31.h>

#include "shader.h"

static void load_file(char*  filename, char** buf);
static void check_shader(GLuint shader);

shader_t
shader_load(char* shader_name) {

    char* v_shader_txt;
    char* f_shader_txt;

    char file_name[128];
    sprintf(file_name, "%s.v.glsl", shader_name);
    load_file(shader_name, &v_shader_txt);
    sprintf(file_name, "%s.f.glsl", shader_name);
    load_file(shader_name, &f_shader_txt);

    // TODO CHECK LOADING

    // VERTEX
    printf("vertext shader\n");
    printf("----\n%s\n----\n", v_shader_txt);

    GLint v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader, 1, (const char**)&v_shader_txt, NULL);
    glCompileShader(v_shader);
    check_shader(v_shader);
    
    // FRAGMENT
    printf("fragment shader\n");
    printf("----\n%s\n----\n", f_shader_txt);

    GLint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, (const char**)&f_shader_txt, NULL);
    glCompileShader(f_shader);
    check_shader(f_shader);

    // PROGRAMM
    GLint program = glCreateProgram();
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);

    GLint mvp, col, pos;
    mvp = glGetUniformLocation(program, "mvp");
    col = glGetUniformLocation(program, "col");
    pos = glGetAttribLocation(program, "pos");

    free(v_shader_txt);
    free(f_shader_txt);

    static GLint mvp_location, vpos_location, col_location;
    return (shader_t) { program, mvp, col, pos };
}

static void check_shader(GLuint shader) {
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        // The maxLength includes the NULL character
        GLchar error[maxLength];
        glGetShaderInfoLog(shader, maxLength, &maxLength, error);
        fprintf(stderr, "ERR: %s\n", error);

        // Provide the infolog in whatever manor you deem best.
        glDeleteShader(shader); // Don't leak the shader.
        return;
    }
}

void shader_clear(shader_t* shader) {
    // TODO cleanup v_shader, f_shader_, program
}

static void
load_file(
    char*  filename,
    char** buf) {

    long length = 0;
    FILE* fp = fopen (filename, "rb");

    if(fp) {
        fseek (fp, 0, SEEK_END);
        length = ftell(fp);
        fseek (fp, 0, SEEK_SET);
        *buf = malloc(length+1);
        if (*buf) {
            fread(*buf, 1, length, fp);
        }
        fclose (fp);
        (*buf)[length]=0;
    }
}
