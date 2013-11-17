add_definitions( -DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

include_directories (${Task_3_SOURCE_DIR}/../task_2/lib)

link_directories(${Task_3_BINARY_DIR}/lib)

add_executable(bad_channel main.cpp)

target_link_libraries(bad_channel deal)
