cmake_minimum_required(VERSION 3.13)
project(airspyhf_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

if (MSVC)
    # Lib path
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a" ".a" ".lib")

    target_link_directories(airspyhf_source PRIVATE "${CMAKE_SOURCE_DIR}/sdk/out/airspyhf")

    target_include_directories(airspyhf_source PUBLIC "${CMAKE_SOURCE_DIR}/sdk/out/airspyhf")

    target_link_libraries(airspyhf_source PRIVATE airspyhf)

    # Copy the airspyhf_source.dll to the output directory
    add_custom_command(TARGET airspyhf_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/airspyhf_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
    add_custom_command(TARGET airspyhf_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/sdk/out/airspyhf/airspyhf.dll"
        "${CMAKE_SOURCE_DIR}/sdk/out/bin/pthreadVC3.dll"
		$<TARGET_FILE_DIR:sdrpp>/
	)
elseif (ANDROID)
    target_include_directories(airspyhf_source PUBLIC
        /sdr-kit/${ANDROID_ABI}/include/libairspyhf
    )

    target_link_libraries(airspyhf_source PUBLIC
        /sdr-kit/${ANDROID_ABI}/lib/libusb1.0.so
        /sdr-kit/${ANDROID_ABI}/lib/libairspyhf.so
    )
else (MSVC)
    find_package(PkgConfig)

    pkg_check_modules(LIBAIRSPYHF REQUIRED libairspyhf)

    target_include_directories(airspyhf_source PRIVATE ${LIBAIRSPYHF_INCLUDE_DIRS})
    target_link_directories(airspyhf_source PRIVATE ${LIBAIRSPYHF_LIBRARY_DIRS})
    target_link_libraries(airspyhf_source PRIVATE ${LIBAIRSPYHF_LIBRARIES})

    # Include it because for some reason pkgconfig doesn't look here?
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        target_include_directories(airspyhf_source PRIVATE "/usr/local/include")
    endif()
    
endif ()