add_definitions( -DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package( Boost REQUIRED COMPONENTS system)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(message message.cpp)
    target_link_libraries(message ${Boost_LIBRARIES})
else()
    message("Unable to find 'Boost'")
endif()
