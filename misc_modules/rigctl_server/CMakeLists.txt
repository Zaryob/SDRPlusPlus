cmake_minimum_required(VERSION 3.13)
project(rigctl_server)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

target_include_directories(rigctl_server PRIVATE "src/")
target_include_directories(rigctl_server PRIVATE "../recorder/src")
target_include_directories(rigctl_server PRIVATE "../../decoder_modules/meteor_demodulator/src")
target_include_directories(rigctl_server PRIVATE "../../decoder_modules/radio/src")
# Copy the rigctl_server.dll to the output directory
add_custom_command(TARGET rigctl_server POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_BINARY_DIR}/rigctl_server.dll"
	$<TARGET_FILE_DIR:sdrpp>/modules
)