cmake_minimum_required(VERSION 3.13)
project(hackrf_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

if (MSVC)
    # Lib path
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a" ".a" ".lib")

    target_link_directories(hackrf_source PRIVATE  "${CMAKE_SOURCE_DIR}/sdk/out/bin")
    
    target_include_directories(hackrf_source PUBLIC "${CMAKE_SOURCE_DIR}/sdk/out/include")

    target_link_libraries(hackrf_source PRIVATE hackrf)
    
    # Copy the audio_source.dll to the output directory
    add_custom_command(TARGET hackrf_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/hackrf_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
    add_custom_command(TARGET hackrf_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/sdk/out/bin/hackrf.dll"
		$<TARGET_FILE_DIR:sdrpp>/
	)
elseif (ANDROID)
    target_include_directories(hackrf_source PUBLIC
        /sdr-kit/${ANDROID_ABI}/include/libhackrf
    )

    target_link_libraries(hackrf_source PUBLIC
        /sdr-kit/${ANDROID_ABI}/lib/libusb1.0.so
        /sdr-kit/${ANDROID_ABI}/lib/libhackrf.so
    )
else (MSVC)
    find_package(PkgConfig)

    pkg_check_modules(LIBHACKRF REQUIRED libhackrf)

    target_include_directories(hackrf_source PRIVATE ${LIBHACKRF_INCLUDE_DIRS})
    target_link_directories(hackrf_source PRIVATE ${LIBHACKRF_LIBRARY_DIRS})
    target_link_libraries(hackrf_source PRIVATE ${LIBHACKRF_LIBRARIES})
endif ()