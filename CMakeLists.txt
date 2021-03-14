cmake_minimum_required(VERSION 3.16)
project(SFTwx)

set(CMAKE_CXX_STANDARD 14)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})


include_directories(include)


add_executable(SFTwx
        src/main.cpp src/MainFrame.cpp)

target_link_libraries(SFTwx ${wxWidgets_LIBRARIES})
