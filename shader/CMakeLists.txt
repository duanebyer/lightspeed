cmake_minimum_required(VERSION 2.8)
set(
  SHADERS
  render_relativistic.vert
  render_relativistic.frag
)

foreach(item IN LISTS SHADERS)
  message(STATUS ${item})
  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
    COMMAND ${CMAKE_COMMAND} -E copy
      "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
      "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${item}"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
  )
endforeach()

add_custom_target(shaders-target ALL DEPENDS ${SHADERS})

