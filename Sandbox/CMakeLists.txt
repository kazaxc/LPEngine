# CMakeList.txt : CMake file for Sandbox

cmake_minimum_required (VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the project name and version
project(Sandbox VERSION 0.0.1)

# Set the source files
set(SOURCE_FILES
    "src/TestingApp.h"
    "src/TestingApp.cpp"
	"src/main.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Target link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE LPEngineCore)

target_include_directories(Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Copy the assets folder to the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Copy the dlls to the build directory
set(SOURCE_DLL_DIR "${CMAKE_BINARY_DIR}/LPEngineCore/")

if(WIN32)
    set_target_properties(Sandbox PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    file(GLOB DLL_FILES "${SOURCE_DLL_DIR}/*.dll")
    foreach(DLL_FILE IN LISTS DLL_FILES)
        add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_FILE}"
        "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Copying dlls to build directory"
        )
    endforeach()
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()