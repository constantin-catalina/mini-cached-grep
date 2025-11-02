cmake_minimum_required(VERSION 3.2)
project(mini-grep)

if(CMAKE_VERSION GREATER "3.1.2") 
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
else()
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    endif()    
endif() 

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g -O1)
  add_link_options(-fsanitize=address)
elseif (MSVC)
  add_compile_options(/fsanitize=address /Zi)
endif()

include_directories(inc)

set(myfiles src/main.cpp src/Cache.cpp src/FileBuffer.cpp src/Searcher.cpp)

add_executable(mini-grep ${myfiles})
