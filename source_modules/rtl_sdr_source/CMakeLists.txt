cmake_minimum_required(VERSION 3.13)
project(rtl_sdr_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

if (MSVC)
    # Lib path
        # Lib path
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a" ".a" ".lib")

    target_link_directories(rtl_sdr_source PRIVATE  "${CMAKE_SOURCE_DIR}/sdk/out/bin")

    target_link_directories(rtl_sdr_source PRIVATE  "${CMAKE_SOURCE_DIR}/sdk/out/lib")
    
    target_include_directories(rtl_sdr_source PUBLIC "${CMAKE_SOURCE_DIR}/sdk/out/include")

    target_link_libraries(rtl_sdr_source PRIVATE rtlsdr)
    
    # Copy the audio_source.dll to the output directory
    add_custom_command(TARGET rtl_sdr_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/rtl_sdr_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
    add_custom_command(TARGET rtl_sdr_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/sdk/out/bin/rtlsdr.dll"
		$<TARGET_FILE_DIR:sdrpp>/
	)

elseif (ANDROID)
    target_link_libraries(rtl_sdr_source PUBLIC
        /sdr-kit/${ANDROID_ABI}/lib/libusb1.0.so
        /sdr-kit/${ANDROID_ABI}/lib/librtlsdr.so
    )
else (MSVC)
    find_package(PkgConfig)

    pkg_check_modules(LIBRTLSDR REQUIRED librtlsdr)
    pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

    target_include_directories(rtl_sdr_source PRIVATE ${LIBRTLSDR_INCLUDE_DIRS} ${LIBUSB_INCLUDE_DIRS})
    target_link_directories(rtl_sdr_source PRIVATE ${LIBRTLSDR_LIBRARY_DIRS} ${LIBUSB_LIBRARY_DIRS})
    target_link_libraries(rtl_sdr_source PRIVATE ${LIBRTLSDR_LIBRARIES} ${LIBUSB_LIBRARIES})
endif ()