#pragma once

typedef struct shader {
    int program, mvp, col, pos;
} shader_t;

// loading shader from fs
shader_t shaders_load(char* shader_name);
void shader_clear(shader_t* shader);
