cmake_minimum_required(VERSION 3.13)
project(hermes_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(hermes_source PRIVATE "src/")

# Copy the hermes_source.dll to the output directory
add_custom_command(TARGET hermes_source POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/hermes_source.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)