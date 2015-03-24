# Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

project(lfs)

set(GLEW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/glew/)

file(GLOB_RECURSE sources_Game Game/*.cpp Game/*.hpp)
file(GLOB_RECURSE sources_Pool Pool/*.cpp Pool/*.hpp)
file(GLOB_RECURSE sources_Server Server/*.cpp Server/*.hpp)
file(GLOB_RECURSE sources_Shader Shader/*.cpp Shader/*.hpp)
file(GLOB_RECURSE sources_Utils Utils/*.cpp Utils/*.hpp)
file(GLOB_RECURSE sources_Window Window/*.cpp Window/*.hpp)

set(EPS_SRCS
  ${EPS_SRCS}
  ${sources_Game}
  ${sources_Pool}
  ${sources_Server}
  ${sources_Shader}
  ${sources_Utils}
  ${sources_Window}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Game
  ${CMAKE_CURRENT_SOURCE_DIR}/Pool
  ${CMAKE_CURRENT_SOURCE_DIR}/Server
  ${CMAKE_CURRENT_SOURCE_DIR}/Shader
  ${CMAKE_CURRENT_SOURCE_DIR}/Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/Window

  ${CMAKE_SOURCE_DIR}/dep/glfw/include
  ${CMAKE_SOURCE_DIR}/dep/glew/include
)

add_definitions(-DGLEW_STATIC)

add_executable(lfs
  ${EPS_SRCS}
)

target_link_libraries(lfs
  ${THREAD_LIB}
  glfw
  glew_s
  ${GLFW_LIBRARIES}
)

add_custom_command(TARGET lfs 
    COMMAND ${CMAKE_COMMAND} -E remove_directory
    $<TARGET_FILE_DIR:lfs>/resources)

add_custom_command(TARGET lfs
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:lfs>/resources)

#add_custom_target(resources
#    COMMAND ${CMAKE_COMMAND} -E remove_directory
#    $<TARGET_FILE_DIR:lfs>/resources)

#add_custom_command(TARGET resources
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:lfs>/resources)

#add_custom_command(TARGET lfs
#    COMMAND ${CMAKE_COMMAND} -E create_symlink
#    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:lfs>/resources)
