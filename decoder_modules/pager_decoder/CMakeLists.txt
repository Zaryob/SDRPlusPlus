cmake_minimum_required(VERSION 3.13)
project(pager_decoder)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(pager_decoder PRIVATE "src/")

# Copy the pager_decoder.dll to the output directory
add_custom_command(TARGET pager_decoder POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/pager_decoder.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)