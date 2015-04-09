# Based on TrinityCore CMake files
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

project(ModernGameEngine)
get_filename_component(CURRENT_PROJECT_DIRECTORY ${PROJECT_SOURCE_DIR}/.. REALPATH)

file(GLOB_RECURSE sources_Pool Pool/*.cpp           ${CURRENT_PROJECT_DIRECTORY}/include/Pool/*.hpp)
file(GLOB_RECURSE sources_Scheduler Scheduler/*.cpp ${CURRENT_PROJECT_DIRECTORY}/include/Scheduler/*.hpp)
file(GLOB_RECURSE sources_Shader Shader/*.cpp       ${CURRENT_PROJECT_DIRECTORY}/include/Shader/*.hpp)
file(GLOB_RECURSE sources_Utils Utils/*.cpp         ${CURRENT_PROJECT_DIRECTORY}/include/Utils/*.hpp)
file(GLOB_RECURSE sources_Window Window/*.cpp       ${CURRENT_PROJECT_DIRECTORY}/include/Window/*.hpp)

set(EPS_SRCS
  ${EPS_SRCS}
  ${sources_Pool}
  ${sources_Scheduler}
  ${sources_Shader}
  ${sources_Utils}
  ${sources_Window}
)

set(ModernGameEngine_INCLUDE
  ${CMAKE_BINARY_DIR}
  ${CURRENT_PROJECT_DIRECTORY}/include/Pool
  ${CURRENT_PROJECT_DIRECTORY}/include/Scheduler
  ${CURRENT_PROJECT_DIRECTORY}/include/Shader
  ${CURRENT_PROJECT_DIRECTORY}/include/Utils
  ${CURRENT_PROJECT_DIRECTORY}/include/Window

  ${CURRENT_PROJECT_DIRECTORY}/dep/glfw/include
  ${CURRENT_PROJECT_DIRECTORY}/dep/glew/include
  ${CURRENT_PROJECT_DIRECTORY}/dep/glm
)

include_directories(
  ${ModernGameEngine_INCLUDE}
)

set(ModernGameEngine_DEFINITIONS -DGLEW_STATIC)

add_library(ModernGameEngine
  ${EPS_SRCS}
)

set(ModernGameEngine_LIBRARIES
  ${THREAD_LIB}
  glfw
  glew_s
  ${GLFW_LIBRARIES}
  ModernGameEngine
)
