cmake_minimum_required(VERSION 3.13)
project(meteor_demodulator)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(meteor_demodulator PRIVATE "src/")

if (WIN32)

	# Copy the meteor_demodulator.dll to the output directory
	add_custom_command(TARGET meteor_demodulator POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/meteor_demodulator.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
endif ()