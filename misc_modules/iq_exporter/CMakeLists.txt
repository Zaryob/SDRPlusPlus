cmake_minimum_required(VERSION 3.13)
project(iq_exporter)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

# Copy the iq_exporter.dll to the output directory
add_custom_command(TARGET iq_exporter POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/iq_exporter.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)