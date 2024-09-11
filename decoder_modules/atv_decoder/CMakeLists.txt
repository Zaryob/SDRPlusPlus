cmake_minimum_required(VERSION 3.13)
project(atv_decoder)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(atv_decoder PRIVATE "src/")

# Copy the audio_sink.dll to the output directory
add_custom_command(TARGET atv_decoder POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/atv_decoder.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)