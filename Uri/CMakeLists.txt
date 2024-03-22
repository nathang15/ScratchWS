cmake_minimum_required(VERSION 3.28)
set(This Uri)

set(Headers
    include/Uri/Uri.hpp
)

set(Sources
    src/Uri.cpp
)

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)

target_include_directories(${This} PUBLIC include)

add_subdirectory(test)