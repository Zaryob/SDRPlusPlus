cmake_minimum_required(VERSION 3.13)
project(airspy_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

if (MSVC)
    # Lib path
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a" ".a" ".lib")

    target_link_directories(airspy_source PRIVATE "${CMAKE_SOURCE_DIR}/sdk/out/airspy")

    target_include_directories(airspy_source PUBLIC "${CMAKE_SOURCE_DIR}/sdk/out/airspy")

    target_link_libraries(airspy_source PRIVATE airspy)

    # Copy the airspy.dll to the output directory
    add_custom_command(TARGET airspy_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/airspy_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
    add_custom_command(TARGET airspy_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/sdk/out/airspy/airspy.dll"
        "${CMAKE_SOURCE_DIR}/sdk/out/bin/pthreadVC3.dll"
		$<TARGET_FILE_DIR:sdrpp>/
	)

elseif (ANDROID)
    target_include_directories(airspy_source PUBLIC
        /sdr-kit/${ANDROID_ABI}/include/libairspy
    )

    target_link_libraries(airspy_source PUBLIC
        /sdr-kit/${ANDROID_ABI}/lib/libusb1.0.so
        /sdr-kit/${ANDROID_ABI}/lib/libairspy.so
    )
else (MSVC)
    find_package(PkgConfig)

    pkg_check_modules(LIBAIRSPY REQUIRED libairspy)

    target_include_directories(airspy_source PRIVATE ${LIBAIRSPY_INCLUDE_DIRS})
    target_link_directories(airspy_source PRIVATE ${LIBAIRSPY_LIBRARY_DIRS})
    target_link_libraries(airspy_source PRIVATE ${LIBAIRSPY_LIBRARIES})

    # Include it because for some reason pkgconfig doesn't look here?
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        target_include_directories(airspy_source PRIVATE "/usr/local/include")
    endif()

endif ()