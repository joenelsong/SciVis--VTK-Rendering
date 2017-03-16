cmake_minimum_required(VERSION 2.8)

PROJECT(proj7a)
SET(VTK_DIR /home/operatingsystems/VTK-6.3.0)

find_package(VTK REQUIRED)
#find_package(Qt5Widgets)
include(${VTK_USE_FILE})

add_executable(proj7a proj7a)

if(VTK_LIBRARIES)
  target_link_libraries(proj7a ${VTK_LIBRARIES})
else()
  target_link_libraries(proj7a vtkHybrid)
endif()


