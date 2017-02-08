
add_executable(test01 test01.c egl.c scene.c)
target_link_libraries(test01 GL EGL)

add_executable(test02 test02.c egl.c scene.c)
target_link_libraries(test02 GL EGL)
