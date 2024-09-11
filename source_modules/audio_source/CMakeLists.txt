cmake_minimum_required(VERSION 3.13)
project(audio_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

if (MSVC)
    # Lib path
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a" ".a" ".lib")

    target_link_directories(audio_source PRIVATE "${CMAKE_SOURCE_DIR}/sdk/out/lib")

    # Misc headers
    target_include_directories(audio_source PUBLIC "${CMAKE_SOURCE_DIR}/sdk/out/include/rtaudio/")

    target_link_libraries(audio_source PRIVATE rtaudio)

    # Copy the audio_source.dll to the output directory
    add_custom_command(TARGET audio_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/audio_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
    add_custom_command(TARGET audio_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/sdk/out/bin/rtaudio.dll"
		$<TARGET_FILE_DIR:sdrpp>/
	)
else (MSVC)
    find_package(PkgConfig)

    pkg_check_modules(RTAUDIO REQUIRED rtaudio)

    target_include_directories(audio_source PRIVATE ${RTAUDIO_INCLUDE_DIRS})
    target_link_directories(audio_source PRIVATE ${RTAUDIO_LIBRARY_DIRS})
    target_link_libraries(audio_source PRIVATE ${RTAUDIO_LIBRARIES})

endif ()