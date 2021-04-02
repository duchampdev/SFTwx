cmake_minimum_required(VERSION 3.16)
project(SFTwx)
set(CMAKE_CXX_STANDARD 14)


if(CMAKE_CXX_COMPILER MATCHES "w64-mingw")
    message(STATUS "building for win64")
else()
    message(STATUS "building for linux64")
endif()


if(CMAKE_CXX_COMPILER MATCHES "w64-mingw")
    set(wxWidgets_USE_STATIC ON)
endif()


find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
include_directories(include)

add_executable(SFTwx
        src/main.cpp src/MainFrame.cpp)


if(CMAKE_CXX_COMPILER MATCHES "w64-mingw")
    target_link_libraries(SFTwx ${wxWidgets_LIBRARIES} -static)
else()
    target_link_libraries(SFTwx ${wxWidgets_LIBRARIES})
endif()
