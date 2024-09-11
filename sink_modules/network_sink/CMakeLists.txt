cmake_minimum_required(VERSION 3.13)
project(network_sink)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

# Copy the network_sink.dll to the output directory
add_custom_command(TARGET network_sink POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/network_sink.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)