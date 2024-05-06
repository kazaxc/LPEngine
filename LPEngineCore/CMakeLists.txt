# CMakeList.txt : CMake file for LPEngineCore

cmake_minimum_required (VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Link external libraries

# Vulkan SDK
set(VULKAN_SDK_PATH "C:/Dev/Libraries/VulkanSDK/VulkanSDK-1.3.268.0")
set(Vulkan_INCLUDE_DIR "${VULKAN_SDK_PATH}/Include")
set(Vulkan_LIBRARY "${VULKAN_SDK_PATH}/Lib")
find_package(Vulkan REQUIRED)

# SDL2
set(SDL2_DIR "C:/Dev/Libraries/SDL2/SDL2-2.28.5/cmake")
find_package(SDL2 REQUIRED)

# SDL2_image
set(SDL2_image_DIR "C:/Dev/Libraries/SDL2/SDL2_image-2.6.3/cmake")
find_package(SDL2_image REQUIRED)

# glm
set(glm_DIR "C:/Dev/Libraries/glm/glm-0.9.9.8/cmake/glm")
find_package(glm REQUIRED)

# Set the project name
project(LPEngineCore VERSION 0.0.1)

# Set source files
set(SOURCE_FILES
	"include/LPEngine.h"
	"include/core/Defines.h"
	"include/core/Logger.h"
	"src/core/Logger.cpp"
	"include/util/DateTime.h" 
	"include/rendering/Window.h"
	"src/rendering/Window.cpp"
	"include/rendering/Pipeline.h"
	"src/rendering/Pipeline.cpp"
	"include/rendering/Device.h"
	"src/rendering/Device.cpp"
	"include/rendering/Swapchain.h"
	"src/rendering/Swapchain.cpp"
	"include/rendering/Model.h"
	"src/rendering/Model.cpp"
	"include/core/GameObject.h"
	"src/core/GameObject.cpp"
	"include/rendering/Renderer.h"
	"src/rendering/Renderer.cpp"
	"include/rendering/RenderSystem.h"
	"src/rendering/RenderSystem.cpp"
)

# Add source files to the shared library
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Set the dll export macro
target_compile_definitions(${PROJECT_NAME} PRIVATE LPENGINE_BUILD_DLL)

# Link external libraries
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_image_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${Vulkan_LIBRARY})

target_link_libraries(${PROJECT_NAME} PUBLIC vulkan-1 ${SDL2_LIBRARIES} SDL2_image::SDL2_image glm::glm)

install(FILES "include/LPEngine.h" DESTINATION include)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "LPEngine")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# Copy the dll files to the build folder
# SDL2
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${SDL2_DIR}/../lib/x64/SDL2.dll"
	$<TARGET_FILE_DIR:${PROJECT_NAME}>)

# SDL2_image
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${SDL2_image_DIR}/../lib/x64/SDL2_image.dll"
	$<TARGET_FILE_DIR:${PROJECT_NAME}>)