add_subdirectory(glfw)

find_package(GLEW)

if (NOT ${GLEW_FOUND})
    add_subdirectory(glew)
endif()
