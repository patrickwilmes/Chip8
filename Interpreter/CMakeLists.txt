set(SOURCES
        Chip8.cpp
        Chip8.h
        Memory.cpp
        Memory.h
        DisplayBuffer.cpp
        DisplayBuffer.h
        Cpu.cpp
        Cpu.h
        main.cpp
        )

add_executable(Chip8 ${SOURCES})
target_link_libraries(Chip8 LibGraphics)

if(DEFINED USE_MEM_ASSERT)
    target_compile_definitions(Chip8 PRIVATE USE_MEM_ASSERT)
endif()