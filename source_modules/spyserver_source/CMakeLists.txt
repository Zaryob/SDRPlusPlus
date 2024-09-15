cmake_minimum_required(VERSION 3.13)
project(spyserver_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(spyserver_source PRIVATE "src/")

if(WIN32)

	# Copy the spyserver_source.dll to the output directory
	add_custom_command(TARGET spyserver_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/spyserver_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
endif()