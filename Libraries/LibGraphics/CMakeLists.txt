set(SOURCES
        Window.cpp
        Window.h

        Common.h
        )

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)

add_library(LibGraphics ${SOURCES})
target_link_libraries(LibGraphics PRIVATE SDL2::SDL2main SDL2::SDL2-static SDL2::SDL2_image SDL2::SDL2_ttf LibCommon)
#TODO: adding the include directory for LibCommon like this is a dirty hack and should be cleaned up
target_include_directories(LibGraphics PUBLIC . ${CMAKE_SOURCE_DIR}/Libraries/LibCommon/)