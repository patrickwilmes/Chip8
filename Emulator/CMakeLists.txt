set(SOURCES
        Chip8.cpp
        Chip8.h
        Memory.cpp
        Memory.h
        Display.cpp
        Display.h
        Cpu.cpp
        Cpu.h
        main.cpp
        )

add_executable(Emulator ${SOURCES})
target_link_libraries(Emulator LibGraphics)
