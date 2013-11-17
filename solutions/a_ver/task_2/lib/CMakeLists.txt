add_definitions( -DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

#include_directories(${Task_2_SOURCE_DIR}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(deal deal.cpp)
