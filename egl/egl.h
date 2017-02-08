#pragma once

#define BUFFER_WIDTH  512
#define BUFFER_HEIGHT 512

void egl_init();
void egl_swap();
void egl_save(char const* filename);
void egl_close();
