add_subdirectory(glfw)
add_subdirectory(glm)

find_package(GLEW)
if (NOT ${GLEW_FOUND})
    add_subdirectory(glew)
endif()
