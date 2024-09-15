cmake_minimum_required(VERSION 3.13)
project(recorder)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(recorder PRIVATE "src/")
target_include_directories(recorder PRIVATE "../../decoder_modules/radio/src")

if(WIN32)
	# Copy the recorder.dll to the output directory
	add_custom_command(TARGET recorder POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/recorder.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
endif()