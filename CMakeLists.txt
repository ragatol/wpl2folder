cmake_minimum_required(VERSION 3.26)

project(wpl2folder VERSION 0.1
	DESCRIPTION "Creates a folder with all music from many Windows Media Player playlists files, with volume normalized and converted to .mp3"
	LANGUAGES CXX)

# Setup c++ version for the whole project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Increase warning level on debug builds
if (MSVC)
	string(APPEND CMAKE_CXX_FLAGS_DEBUG " /W4")
else()
	string(APPEND CMAKE_CXX_FLAGS_DEBUG " -Wall -Wextra -Wpedantic")
endif()

# Configure paths for the required libraries
find_package(nowide CONFIG REQUIRED)
find_path(PORTABLE_FILE_DIALOGS_INCLUDE_DIRS "portable-file-dialogs.h")

# Export compile_commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Configure target
add_executable(wpl2folder main.cpp)

# add -DLANG_PTBR=1 to build the brazilian portuguese version
if(DEFINED LANG_PTBR)
	target_compile_definitions(wpl2folder PRIVATE LANG_PTBR)
endif()

# link required libraries
target_link_libraries(wpl2folder PRIVATE nowide::nowide)
target_include_directories(wpl2folder PRIVATE ${PORTABLE_FILE_DIALOGS_INCLUDE_DIRS})
