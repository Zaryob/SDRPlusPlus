cmake_minimum_required(VERSION 3.13)
project(network_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

if(WIN32)
	# Copy the airspyhf_source.dll to the output directory
	add_custom_command(TARGET network_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/network_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
endif()