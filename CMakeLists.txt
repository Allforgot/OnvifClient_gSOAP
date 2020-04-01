CMAKE_MINIMUM_REQUIRED(VERSION 3.4.1)
project(OnvifClient_gSOAP)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-DWITH_OPENSSL -DWITH_DOM")

include_directories(${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/custom
        ${PROJECT_SOURCE_DIR}/include/plugin
        ${PROJECT_SOURCE_DIR}/include/soapProxy
        )

aux_source_directory(${PROJECT_SOURCE_DIR}/src src)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/custom custom_src)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/plugin plugin_src)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/soapProxy soapProxy_src)

include_directories(${PROJECT_SOURCE_DIR}/3rdparty/include)
link_directories("${PROJECT_SOURCE_DIR}/3rdparty/lib/openssl")

set(sourcecode
        ${src}
        ${custom_src}
        ${plugin_src}
        ${soapProxy_src}
        )

add_subdirectory(samples/deviceBinding)
add_subdirectory(samples/mediaBinding)
add_subdirectory(samples/ptzBinding)
#add_subdirectory(samples/remoteDiscovery)
