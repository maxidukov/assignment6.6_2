cmake_minimum_required(VERSION 3.5)

project(assignment6_6__2_QC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(assignment6_6__2_QC main.cpp
    all_classes.h)
target_link_libraries(assignment6_6__2_QC libwtdbo.so libwtdbopostgres.so)

include(GNUInstallDirs)
# install(TARGETS assignment6_6__2_QC
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
