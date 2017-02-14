#pragma once

#define BUFFER_WIDTH  1280
#define BUFFER_HEIGHT  720

int egl_init();
void egl_swap();
void egl_save(char const* filename);

void egl_init_stream();
void egl_write_stream();
void egl_close_stream();

void egl_close();
