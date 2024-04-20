

cmake_minimum_required(VERSION 3.13)

include(${PICO_SDK_INIT_CMAKE_FILE})

#project(smell-sound)
add_library(ssd1306 INTERFACE)
target_sources(ssd1306 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/ssd1306.c)
target_include_directories(ssd1306 INTERFACE include)
target_link_libraries(${PROJECT_NAME} pico_stdlib)