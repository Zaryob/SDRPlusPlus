cmake_minimum_required(VERSION 3.13)
project(scanner)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(scanner PRIVATE "src/")
# Copy the scanner.dll to the output directory
add_custom_command(TARGET scanner POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/scanner.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)