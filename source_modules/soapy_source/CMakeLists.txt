cmake_minimum_required(VERSION 3.13)
project(soapy_source)

file(GLOB SRC "src/*.cpp")

include(${SDRPP_MODULE_CMAKE})

if (MSVC)
    # Lib path
    target_link_directories(soapy_source PRIVATE  "${CMAKE_SOURCE_DIR}/sdk/out/lib/")
    target_include_directories(soapy_source PRIVATE  "${CMAKE_SOURCE_DIR}/sdk/out/include")
    # Misc headers

    target_link_libraries(soapy_source PRIVATE SoapySDR)
    add_custom_command(TARGET soapy_source POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_BINARY_DIR}/soapy_source.dll"
		$<TARGET_FILE_DIR:sdrpp>/modules
	)
    add_custom_command(TARGET soapy_source POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/sdk/out/bin/SoapySDR.dll"
	$<TARGET_FILE_DIR:sdrpp>/
)
else (MSVC)
    find_package(PkgConfig)

    pkg_check_modules(SOAPY REQUIRED SoapySDR)

    target_include_directories(soapy_source PRIVATE ${SOAPY_INCLUDE_DIRS})
    target_link_directories(soapy_source PRIVATE ${SOAPY_LIBRARY_DIRS})
    target_link_libraries(soapy_source PRIVATE ${SOAPY_LIBRARIES})
endif ()