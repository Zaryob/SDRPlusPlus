cmake_minimum_required(VERSION 3.13)
project(badgesdr_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

if (MSVC)
    target_link_directories(badgesdr_source PRIVATE "${CMAKE_SOURCE_DIR}/sdk/out/lib")
    target_include_directories(badgesdr_source PRIVATE ${CMAKE_SOURCE_DIR}/sdk/out/include)
    target_link_libraries(badgesdr_source PRIVATE libusb-1.0)

    # Copy the badgesdr_source.dll to the output directory
    add_custom_command(TARGET badgesdr_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/badgesdr_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
elseif (ANDROID)
    target_link_libraries(badgesdr_source PUBLIC
        /sdr-kit/${ANDROID_ABI}/lib/libusb1.0.so
        /sdr-kit/${ANDROID_ABI}/lib/librtlsdr.so
    )
else (MSVC)
    find_package(PkgConfig)

    pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

    target_include_directories(badgesdr_source PRIVATE ${LIBUSB_INCLUDE_DIRS})
    target_link_directories(badgesdr_source PRIVATE ${LIBUSB_LIBRARY_DIRS})
    target_link_libraries(badgesdr_source PRIVATE ${LIBUSB_LIBRARIES})
endif ()