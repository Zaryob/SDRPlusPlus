cmake_minimum_required(VERSION 3.13)
project(scheduler)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(scheduler PRIVATE "src/")
# Copy the scheduler.dll to the output directory
add_custom_command(TARGET scheduler POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/scheduler.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)