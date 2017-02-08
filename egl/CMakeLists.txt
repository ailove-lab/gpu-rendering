
add_executable(test test.c egl.c scene.c)
target_link_libraries(test GL EGL)
