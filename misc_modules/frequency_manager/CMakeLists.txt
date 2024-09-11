cmake_minimum_required(VERSION 3.13)
project(frequency_manager)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(frequency_manager PRIVATE "src/" "../../decoder_modules/radio/src")

# Copy the iq_exporter.dll to the output directory
add_custom_command(TARGET frequency_manager POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/frequency_manager.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)