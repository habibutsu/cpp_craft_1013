add_definitions( -DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )
add_definitions( -DBINARY_DIR="${BINARIES_DIRECTORY}" )

include_directories (${Task_4_SOURCE_DIR}/4_4)

add_library(lib_4_4 
    ${Task_4_SOURCE_DIR}/4_4/a_message.cpp
    ${Task_4_SOURCE_DIR}/4_4/b_message.cpp
    ${Task_4_SOURCE_DIR}/4_4/message.cpp
    ${Task_4_SOURCE_DIR}/4_4/message_reader.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (Boost_USE_STATIC_LIBS ON)
find_package( Boost REQUIRED COMPONENTS system unit_test_framework filesystem)

if(Boost_FOUND)
    
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(test_4_4
        a_message_tests.cpp
        b_message_tests.cpp
        message_reader_tests.cpp
        message_tests.cpp
        test_registrator.cpp)
    target_link_libraries(test_4_4 ${Boost_LIBRARIES} lib_4_4)
    enable_testing()
    add_test(test_4_4 test_4_4)
else()
    message("Unable to find 'Boost'")
endif()