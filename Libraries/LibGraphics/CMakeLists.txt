set(SOURCES
        LibTest.cpp
        LibTest.h
        )

add_library(LibGraphics ${SOURCES})
target_include_directories(LibGraphics PUBLIC .)