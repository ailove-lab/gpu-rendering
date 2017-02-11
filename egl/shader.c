#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <GLES3/gl31.h>

#include "shader.h"

static void file_load(char*  filename, char** buf);
static void shader_check(GLuint shader);

shader_t
shader_load(char* shader_name) {

    shader_t shader;

    char* v_shader_txt;
    char* f_shader_txt;

    char file_name[128];
    sprintf(file_name, "%s.v.glsl", shader_name);
    file_load(file_name, &v_shader_txt);
    sprintf(file_name, "%s.f.glsl", shader_name);
    file_load(file_name, &f_shader_txt);

    // VERTEX
    printf("vertext shader\n");
    printf("----\n%s\n----\n", v_shader_txt);

    GLint v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader, 1, (const char**)&v_shader_txt, NULL);
    glCompileShader(v_shader);
    shader_check(v_shader);
    
    // FRAGMENT
    printf("fragment shader\n");
    printf("----\n%s\n----\n", f_shader_txt);

    GLint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, (const char**)&f_shader_txt, NULL);
    glCompileShader(f_shader);
    shader_check(f_shader);

    // PROGRAM
    shader.program = glCreateProgram();
    glAttachShader(shader.program, v_shader);
    glAttachShader(shader.program, f_shader);
    glLinkProgram(shader.program);

    shader.mvp = glGetUniformLocation(shader.program, "mvp");
    shader.col = glGetUniformLocation(shader.program, "col");
    shader.pos = glGetAttribLocation(shader.program, "pos");

    // it's good practice to delete shaders righ after linking to program
    glDeleteShader(f_shader);
    glDeleteShader(v_shader);
    free(v_shader_txt);
    free(f_shader_txt);
    
    return shader;
}


static void
shader_check(GLuint shader) {
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
    glDeleteProgram(shader->program);
}

static void
file_load(
    char*  filename,
    char** buf) {

    long length = 0;
    FILE* fp = fopen (filename, "rb");
    assert(fp>0);
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
