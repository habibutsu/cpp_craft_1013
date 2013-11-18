add_definitions( -DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )
add_definitions( -DBINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )

set(COMPONENT_NAME 4_4)
set(SOURCE_DIR ${Task_4_SOURCE_DIR}/${COMPONENT_NAME}/)

include_directories (${SOURCE_DIR})

add_library(lib_${COMPONENT_NAME}
    ${SOURCE_DIR}a_message.cpp
    ${SOURCE_DIR}b_message.cpp
    ${SOURCE_DIR}message.cpp
    ${SOURCE_DIR}message_reader.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (Boost_USE_STATIC_LIBS ON)
find_package( Boost REQUIRED COMPONENTS system unit_test_framework filesystem)

if(Boost_FOUND)

    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(test_${COMPONENT_NAME}
        a_message_tests.cpp
        b_message_tests.cpp
        message_reader_tests.cpp
        message_tests.cpp
        test_registrator.cpp)
    target_link_libraries(
        test_${COMPONENT_NAME} ${Boost_LIBRARIES} lib_${COMPONENT_NAME})
    enable_testing()
    add_test(test_${COMPONENT_NAME} test_${COMPONENT_NAME})
else()
    message("Unable to find 'Boost'")
endif()