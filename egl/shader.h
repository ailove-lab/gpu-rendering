#pragma once
// Shader here is casual name for composition of glsl program and params compiled from glsl files 
typedef struct shader {
    int program, mvp, col, pos;
} shader_t;

// loading shader from fs
shader_t shader_load(char* shader_name);
void shader_clear(shader_t* shader);
