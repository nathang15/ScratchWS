cmake_minimum_required(VERSION 3.28)
set(This UriTests)

set(Sources
    src/UriTests.cpp
)

add_executable(${This} ${Sources})
set_target_properties(${This} PROPERTIES
    FOLDER Tests
)

target_link_libraries(${This} PUBLIC
    gtest_main
    Uri
)

add_test(
    NAME ${This}
    COMMAND ${This}
)