cmake_minimum_required(VERSION 3.13)
project(usrp_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

if (MSVC)
    # Lib path
    target_link_directories(usrp_source PRIVATE  "${CMAKE_SOURCE_DIR}/sdk/out/bin")
    target_link_directories(usrp_source PRIVATE  "${CMAKE_SOURCE_DIR}/sdk/out/lib")
    
    target_include_directories(usrp_source PUBLIC "${CMAKE_SOURCE_DIR}/sdk/out/include"  "${CMAKE_SOURCE_DIR}/sdk/out/include/boost-1_86")

    target_link_libraries(usrp_source PRIVATE uhd)
else (MSVC)
    find_package(PkgConfig)

    pkg_check_modules(LIBUHD REQUIRED uhd)

    target_include_directories(usrp_source PRIVATE ${LIBUHD_INCLUDE_DIRS})
    target_link_directories(usrp_source PRIVATE ${LIBUHD_LIBRARY_DIRS})
    target_link_libraries(usrp_source PRIVATE ${LIBUHD_LIBRARIES})
endif ()