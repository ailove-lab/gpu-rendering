
cmake_minimum_required(VERSION 3.7)

project(egl_tests)

add_executable(test01 test01.c egl.c scene.c)
target_link_libraries(test01 GLEW GL EGL)

add_executable(test02 test02.c egl.c scene.c)
target_link_libraries(test02 GLEW GL EGL)

#add_executable(test03 test03.c)
#target_link_libraries(test03 GLESv2 EGL gbm)

# include_directories(dep dep/glad)
# add_subdirectory(dep/glad)
add_executable(test04 test04.c)
target_link_libraries(test04 GLESv2 EGL gbm)
