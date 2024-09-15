cmake_minimum_required(VERSION 3.13)
project(file_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(file_source PRIVATE "src/")

if(WIN32)
	# Copy the file_source.dll to the output directory
	add_custom_command(TARGET file_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/file_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
endif ()