cmake_minimum_required(VERSION 3.15)
project(subsystem-changer CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags
if(MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Werror)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

# Add source files
set(SOURCES subsystem-changer.cpp)

# Create executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# MSVC-specific configurations
if(MSVC)
	# Use static runtime libraries
	set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

	# Enable C++17 features including std::filesystem
	target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

	# Link against the filesystem library
	target_link_libraries(${PROJECT_NAME} PRIVATE version.lib)
else()
	# For non-MSVC compilers, link against stdc++fs if necessary
	target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
endif()