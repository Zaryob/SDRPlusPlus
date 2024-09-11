cmake_minimum_required(VERSION 3.13)
project(audio_sink)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

if (MSVC)
    # Lib path
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a" ".a" ".lib")

    target_link_directories(audio_sink PRIVATE "${CMAKE_SOURCE_DIR}/sdk/out/lib")

    # Misc headers
    target_include_directories(audio_sink PUBLIC "${CMAKE_SOURCE_DIR}/sdk/out/include/rtaudio/")

    target_link_libraries(audio_sink PRIVATE rtaudio)
    # Copy the audio_sink.dll to the output directory
    add_custom_command(TARGET audio_sink POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/audio_sink.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
else (MSVC)
    find_package(PkgConfig)

    pkg_check_modules(RTAUDIO REQUIRED rtaudio)

    target_include_directories(audio_sink PRIVATE ${RTAUDIO_INCLUDE_DIRS})
    target_link_directories(audio_sink PRIVATE ${RTAUDIO_LIBRARY_DIRS})
    target_link_libraries(audio_sink PRIVATE ${RTAUDIO_LIBRARIES})

endif ()