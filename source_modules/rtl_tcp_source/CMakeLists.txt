cmake_minimum_required(VERSION 3.13)
project(rtl_tcp_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(rtl_tcp_source PRIVATE "src/")

if(WIN32)

	# Copy the rtl_tcp_source.dll to the output directory
	add_custom_command(TARGET rtl_tcp_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/rtl_tcp_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
endif ()