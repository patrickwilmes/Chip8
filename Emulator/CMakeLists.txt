set(SOURCES
        main.cpp
        )

add_executable(Emulator ${SOURCES})
target_link_libraries(Emulator LibGraphics)
