
set(PROJECT_NAME 9SliceScale)

add_definitions(-DGLEW_STATIC)

add_executable(${PROJECT_NAME} 
    main.cpp
	my_window.h
	my_window.cpp
	simple_9_slice.h
	simple_9_slice.cpp
    res/shader.vert
    res/shader.frag
)
target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res")
target_compile_definitions(${PROJECT_NAME} PRIVATE RELEASE_RESOURCE_PATH="./res")

set(ALL_LIBS
    debug "libglew32d" optimized "libglew32"
    debug "glfw3_d" optimized "glfw3"
	debug "libimguid" optimized "libimgui"
    ${OPENGL_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})

add_dependencies(${PROJECT_NAME} glew glfw glm stb imgui)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/out/bin )
install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
install(FILES res/shader.vert res/shader.frag res/frame.png DESTINATION ${CMAKE_INSTALL_PREFIX}/res )