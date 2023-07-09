cmake_minimum_required(VERSION 3.12)

project(wpl2folder VERSION 0.1
	DESCRIPTION "Creates a folder with all music from many Windows Media Player playlists files, with volume normalized and converted to .mp3"
	LANGUAGES CXX)

# Setup c++ version for the whole project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Path to the vcpkg.cmake file
include(~/.local/vcpkg/scripts/buildsystems/vcpkg.cmake)

# Configure paths for the required libraries
find_package(fmt CONFIG REQUIRED)
find_package(nowide CONFIG REQUIRED)
find_path(PORTABLE_FILE_DIALOGS_INCLUDE_DIRS "portable-file-dialogs.h")

# Export compile_commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS True)


# Configure target
add_executable(wpl2folder main.cpp)

# sets NDEBUG macro if CMAKE_BUILD_TYPE is set to Debug (for assert and others)
target_compile_definitions(wpl2folder PRIVATE $<$<CONFIG:Debug>:NDEBUG>)

# uncomment following line to compile with pt-br messages
#target_compile_definitions(wpl2folder PRIVATE LANG_PTBR)

# link required libraries
target_link_libraries(wpl2folder PRIVATE fmt::fmt)
target_link_libraries(wpl2folder PRIVATE nowide::nowide)
target_include_directories(wpl2folder PRIVATE ${PORTABLE_FILE_DIALOGS_INCLUDE_DIRS})
