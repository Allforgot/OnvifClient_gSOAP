CMAKE_MINIMUM_REQUIRED(VERSION 3.4.1)
project(remoteDiscovery)
message("Make sample ${PROJECT_NAME}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)

add_executable(${PROJECT_NAME} remoteDiscovery.cpp ${CMAKE_SOURCE_DIR}/src/wsddClient.cpp ${sourcecode})
target_link_libraries(${PROJECT_NAME} ssl crypto)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})