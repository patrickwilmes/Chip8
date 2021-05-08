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

add_executable(Chip8 ${SOURCES})
target_link_libraries(Chip8 LibGraphics)
