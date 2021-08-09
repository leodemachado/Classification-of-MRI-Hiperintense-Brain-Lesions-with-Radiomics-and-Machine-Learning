cmake_minimum_required(VERSION 2.8)

project(BiasCorrectionHistMatching)
 
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
 
add_executable(BiasCorrectionHistMatching MACOSX_BUNDLE BiasCorrectionHistMatching.cxx)
target_link_libraries(BiasCorrectionHistMatching ${ITK_LIBRARIES})
