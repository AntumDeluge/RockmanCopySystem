
cmake_minimum_required(VERSION 3.21)

project("Rockman Copy System" VERSION 0.5)

set(CMAKE_CXX_STANDARD 98 CACHE STRING "C++ standard to be used")

# TODO: add PSP build

file(GLOB APP_SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")

find_package(SDL REQUIRED)
find_package(SDL2 REQUIRED)

if(NOT SDL_FOUND AND NOT SDL2_FOUND)
	message(FATAL_ERROR "Failed to locate either SDL or SDL2")
endif()

if(SDL_FOUND)
	add_executable(rcs ${APP_SRC})
	target_include_directories(rcs PRIVATE
		"${PROJECT_SOURCE_DIR}/src"
		"${SDL_INCLUDE_DIR}"
	)
	target_link_libraries(rcs PRIVATE
		${SDL_LIBRARIES}
	)
endif()

if(SDL2_FOUND)
	add_executable(rcs-sdl2 ${APP_SRC})
	target_compile_definitions(rcs-sdl2 PUBLIC SDL2)
	target_include_directories(rcs-sdl2 PRIVATE
		"${PROJECT_SOURCE_DIR}/src"
		"${SDL2_INCLUDE_DIR}"
	)
	target_link_libraries(rcs-sdl2 PRIVATE
		${SDL2_LIBRARIES}
	)
endif()
