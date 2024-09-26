cmake_minimum_required(VERSION 3.13)
project(sdrpp_core)

if (USE_INTERNAL_LIBCORRECT)
    add_subdirectory("libcorrect/")
endif (USE_INTERNAL_LIBCORRECT)

if (USE_BUNDLE_DEFAULTS)
add_definitions(-DIS_MACOS_BUNDLE)
endif (USE_BUNDLE_DEFAULTS)

# Main code
file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")

add_definitions(-DSDRPP_IS_CORE)
add_definitions(-DFLOG_ANDROID_TAG="SDR++")
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

# Configure backend sources
if (OPT_BACKEND_GLFW)
    file(GLOB_RECURSE BACKEND_SRC "backends/glfw/*.cpp" "backends/glfw/*.c")
endif (OPT_BACKEND_GLFW)
if (OPT_BACKEND_ANDROID)
    file(GLOB_RECURSE BACKEND_SRC "backends/android/*.cpp" "backends/android/*.c")
    set(BACKEND_SRC ${BACKEND_SRC} ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
endif (OPT_BACKEND_ANDROID)

# Add code to dyn lib
add_library(sdrpp_core SHARED ${SRC} ${BACKEND_SRC})

# Set compiler options
target_compile_options(sdrpp_core PRIVATE ${SDRPP_COMPILER_FLAGS})

# Set the install prefix
target_compile_definitions(sdrpp_core PUBLIC INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")

# Include core headers
target_include_directories(sdrpp_core PUBLIC "src/")
target_include_directories(sdrpp_core PUBLIC "src/imgui")




# Configure backend includes and libraries
if (OPT_BACKEND_GLFW)
    target_include_directories(sdrpp_core PUBLIC "backends/glfw")
    target_include_directories(sdrpp_core PUBLIC "backends/glfw/imgui")

    if(!WIN32)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GLFW3 REQUIRED glfw3)
        target_include_directories(sdrpp_core PUBLIC ${GLFW3_INCLUDE_DIRS})
        target_link_directories(sdrpp_core PUBLIC ${GLFW3_LIBRARY_DIRS})
        target_link_libraries(sdrpp_core PUBLIC ${GLFW3_LIBRARIES})
    endif()

endif (OPT_BACKEND_GLFW)

if (OPT_BACKEND_ANDROID)
    target_include_directories(sdrpp_core PUBLIC "backends/android")
    target_include_directories(sdrpp_core PUBLIC "backends/android/imgui")
endif (OPT_BACKEND_ANDROID)

# Link to libcorrect
if (USE_INTERNAL_LIBCORRECT)
    target_include_directories(sdrpp_core PUBLIC "libcorrect/include")
    target_link_libraries(sdrpp_core PUBLIC correct)
endif (USE_INTERNAL_LIBCORRECT)

if (OPT_OVERRIDE_STD_FILESYSTEM)
    target_include_directories(sdrpp_core PUBLIC "std_replacement")
endif (OPT_OVERRIDE_STD_FILESYSTEM)

if (ANDROID)
    target_include_directories(sdrpp_core PUBLIC
        /sdr-kit/${ANDROID_ABI}/include
        ${ANDROID_NDK}/sources/android/native_app_glue
    )

    target_link_libraries(sdrpp_core PUBLIC
        /sdr-kit/${ANDROID_ABI}/lib/libvolk.so
        /sdr-kit/${ANDROID_ABI}/lib/libfftw3f.so
        /sdr-kit/${ANDROID_ABI}/lib/libzstd.so
        android
        EGL
        GLESv3
        log
    )
else()

    # Volk
    if(WIN32)
        add_subdirectory(submodules/volk)
        target_link_libraries(sdrpp_core PUBLIC volk)

    else()
        find_package(PkgConfig REQUIRED)

        pkg_check_modules(VOLK REQUIRED volk)
        target_include_directories(sdrpp_core PUBLIC
                ${VOLK_INCLUDE_DIRS}
        )

        target_link_directories(sdrpp_core PUBLIC
                ${VOLK_LIBRARY_DIRS}
        )

        target_link_libraries(sdrpp_core PUBLIC
                ${VOLK_LIBRARIES}
        )

    endif()


    # OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(sdrpp_core PUBLIC OpenGL::GL)

    # GLFW3
    if(WIN32)
        add_subdirectory(submodules/glfw)
        target_link_libraries(sdrpp_core PUBLIC glfw)

    else()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GLFW3 REQUIRED glfw3)

        target_include_directories(sdrpp_core PUBLIC
                ${GLFW3_INCLUDE_DIRS}
        )

        target_link_directories(sdrpp_core PUBLIC
                ${GLFW3_LIBRARY_DIRS}
        )

        target_link_libraries(sdrpp_core PUBLIC
                ${GLFW3_LIBRARIES}
        )

    endif()

    # FFTW3
    #find_package(FFTW3f CONFIG REQUIRED)
    if(WIN32)
        set (BUILD_SHARED_LIBS OFF)
        set (BUILD_TESTS OFF)
        set (ENABLE_FLOAT ON)
        set (ENABLE_SSE ON)

        set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
        add_subdirectory(submodules/fftw)
        set_target_properties ("fftw3f" PROPERTIES POSITION_INDEPENDET_CODE ON)
        target_include_directories(sdrpp_core PUBLIC ${CMAKE_SOURCE_DIR}/core/submodules/fftw/api)
        target_link_libraries(sdrpp_core PUBLIC fftw3f)
    else()
        find_package(PkgConfig)
        pkg_check_modules(FFTW3 REQUIRED fftw3f)

        target_include_directories(sdrpp_core PUBLIC
                ${FFTW3_INCLUDE_DIRS}
        )

        target_link_directories(sdrpp_core PUBLIC
                ${FFTW3_LIBRARY_DIRS}
        )

        target_link_libraries(sdrpp_core PUBLIC
                ${FFTW3_LIBRARIES}
        )
    endif()

    # WinSock2
    if(WIN32)
        target_link_libraries(sdrpp_core PUBLIC wsock32 ws2_32 iphlpapi)
    endif(WIN32)
    # ZSTD
    if(WIN32)
        add_subdirectory(submodules/zstd/build/cmake)
        target_include_directories(sdrpp_core PUBLIC submodules/zstd/lib)
        target_link_libraries(sdrpp_core PUBLIC libzstd_shared)
    else()
        find_package(PkgConfig)
        pkg_check_modules(LIBZSTD REQUIRED libzstd)
        target_include_directories(sdrpp_core PUBLIC
                ${LIBZSTD_INCLUDE_DIRS}
        )

        target_link_directories(sdrpp_core PUBLIC
                ${LIBZSTD_LIBRARY_DIRS}
        )

        target_link_libraries(sdrpp_core PUBLIC
                ${LIBZSTD_LIBRARIES}
        )
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        target_link_libraries(sdrpp_core PUBLIC stdc++fs)
    endif ()

endif ()

set(CORE_FILES ${RUNTIME_OUTPUT_DIRECTORY} PARENT_SCOPE)

# cmake .. "-DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Install directives
install(TARGETS sdrpp_core DESTINATION lib)
