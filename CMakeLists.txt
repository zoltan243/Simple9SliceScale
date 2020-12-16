cmake_minimum_required(VERSION 3.10)

project(Simple9SliceScale)

find_package(Git REQUIRED)
find_package(OpenGL REQUIRED)

include(ExternalProject)

# GLEW
ExternalProject_Add(glew 
	PREFIX ${CMAKE_BINARY_DIR}/ext SOURCE_SUBDIR build/cmake
	GIT_PROGRESS ON GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git 
	CMAKE_ARGS -DBUILD_UTILS=OFF -DGLEW_STATIC=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/out)

# GLFW
ExternalProject_Add(glfw 
	PREFIX ${CMAKE_BINARY_DIR}/ext
	GIT_PROGRESS ON GIT_TAG 0a49ef0 GIT_REPOSITORY https://github.com/glfw/glfw.git 
	CMAKE_ARGS -DCMAKE_DEBUG_POSTFIX=_d -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/out )

# GLM
ExternalProject_Add(glm 
	PREFIX ${CMAKE_BINARY_DIR}/ext
	GIT_PROGRESS ON GIT_TAG 13724cf GIT_REPOSITORY https://github.com/g-truc/glm.git 
	BUILD_COMMAND "" INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ../glm/glm ${CMAKE_BINARY_DIR}/out/include/glm )

# stb
ExternalProject_Add(stb 
	PREFIX ${CMAKE_BINARY_DIR}/ext
	GIT_PROGRESS ON GIT_REPOSITORY https://github.com/nothings/stb.git 
	CONFIGURE_COMMAND "" BUILD_COMMAND "" INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ../stb/stb_image.h ${CMAKE_BINARY_DIR}/out/include/stb/stb_image.h )

# ImGui
ExternalProject_Add(imgui 
	PREFIX ${CMAKE_BINARY_DIR}/ext SOURCE_SUBDIR ../
	GIT_PROGRESS ON GIT_TAG v1.79 GIT_REPOSITORY https://github.com/ocornut/imgui.git
	CMAKE_ARGS -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/out )

# create a CMakeLists.txt file for imgui (to build it as a lib): CMAKE_BINARY_DIR/ext/src/CMakeLists.txt
file(WRITE ${CMAKE_BINARY_DIR}/ext/src/CMakeLists.txt "cmake_minimum_required(VERSION 3.10) \n\n")
# add core imgui source files:
file(APPEND ${CMAKE_BINARY_DIR}/ext/src/CMakeLists.txt "file(GLOB IMGUI_HEADER_FILES imgui/*.h) \n")
file(APPEND ${CMAKE_BINARY_DIR}/ext/src/CMakeLists.txt "file(GLOB IMGUI_SOURCE_FILES imgui/*.cpp) \n\n")
# add helper files (GLFW and OpenGL3 impl.):
file(APPEND ${CMAKE_BINARY_DIR}/ext/src/CMakeLists.txt "file(GLOB HELPER_HEADER_FILES imgui/examples/imgui_impl_glfw.h imgui/examples/imgui_impl_opengl3.h) \n")
file(APPEND ${CMAKE_BINARY_DIR}/ext/src/CMakeLists.txt "file(GLOB HELPER_SOURCE_FILES imgui/examples/imgui_impl_glfw.cpp imgui/examples/imgui_impl_opengl3.cpp) \n\n")
# include directory for the imgui library project
file(APPEND ${CMAKE_BINARY_DIR}/ext/src/CMakeLists.txt "include_directories(imgui imgui/examples \"${CMAKE_BINARY_DIR}/out/include\") \n")
# creating the library
file(APPEND ${CMAKE_BINARY_DIR}/ext/src/CMakeLists.txt "add_library(libimgui STATIC \$\{IMGUI_SOURCE_FILES\} \$\{HELPER_SOURCE_FILES\}) \n\n")
# install (copy) the lib and header files
file(APPEND ${CMAKE_BINARY_DIR}/ext/src/CMakeLists.txt "install(TARGETS libimgui LIBRARY DESTINATION lib) \n")
file(APPEND ${CMAKE_BINARY_DIR}/ext/src/CMakeLists.txt "install(FILES \$\{IMGUI_HEADER_FILES\} \$\{HELPER_HEADER_FILES\} DESTINATION include/imgui) \n")


include_directories("${CMAKE_BINARY_DIR}/out/include" )
link_directories("${CMAKE_BINARY_DIR}/out/lib")

add_subdirectory(src)