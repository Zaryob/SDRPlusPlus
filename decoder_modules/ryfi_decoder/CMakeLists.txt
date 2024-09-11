cmake_minimum_required(VERSION 3.13)
project(ryfi_decoder)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(ryfi_decoder PRIVATE "src/")

# Copy the ryfi_decoder.dll to the output directory
add_custom_command(TARGET ryfi_decoder POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/ryfi_decoder.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)