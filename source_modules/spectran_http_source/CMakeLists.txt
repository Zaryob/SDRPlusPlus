cmake_minimum_required(VERSION 3.13)
project(spectran_http_source)

file(GLOB_RECURSE SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(spectran_http_source PRIVATE "src/")

# Copy the spectran_http_source.dll to the output directory
add_custom_command(TARGET spectran_http_source POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/spectran_http_source.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)