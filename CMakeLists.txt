cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
OPTION(USE_EMSCRIPTEN "Use Emscripten"	OFF)
if(USE_EMSCRIPTEN)
    if(NOT EMSCRIPTEN_ROOT_PATH)
        message(FATAL_ERROR "Could not locate the Emscripten compiler toolchain directory! Set -DEMSCRIPTEN_ROOT_PATH=<path/to/emsdk>/upstream/emscripten to CMake to explicitly specify the location of the compiler!")
    else()
        message("In Emscripten world...")
        get_filename_component(EMSCRIPTEN_ROOT_PATH "${EMSCRIPTEN_ROOT_PATH}" ABSOLUTE)
        set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_ROOT_PATH}/cmake/Modules/Platform/Emscripten.cmake)
    endif(NOT EMSCRIPTEN_ROOT_PATH)
else()
    message("Emscripten is OFF...")
    #set(VCPKG_TARGET_TRIPLET "x64-windows")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

project(itchEngine)

if(NOT USE_EMSCRIPTEN)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
endif()

set(SOURCE_FILES src/main.cpp)
if(NOT USE_EMSCRIPTEN)
    list(APPEND SOURCE_FILES thirdparty/glad/src/glad.c)
endif()

add_executable(itchEngine ${SOURCE_FILES})

if(USE_EMSCRIPTEN)
    set_target_properties(itchEngine PROPERTIES LINK_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS=[\"png\"] -s USE_WEBGL2=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1 --preload-file ${CMAKE_SOURCE_DIR}/assets --bind" )
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    configure_file(web/index.html ${CMAKE_BINARY_DIR}/index.html)
else()
    target_include_directories(itchEngine PRIVATE thirdparty/glad/include)
endif()

target_include_directories(itchEngine INTERFACE src ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(itchEngine ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_command(TARGET itchEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()
