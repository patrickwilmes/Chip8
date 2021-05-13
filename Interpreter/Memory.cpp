// Copyright (c) 2021, Patrick Wilmes <patrick.wilmes@bit-lake.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "Memory.h"
#include <Assert.h>
#include <cstddef>
#include <iostream>

using namespace Common;

const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_STAT_ADDRESS = 0x50;

uint8_t fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::MemoryManager::MemoryManager()
{
    reset_memory();
    load_fontset();
}

void Chip8::MemoryManager::reset_memory()
{
    for (unsigned char& i : m_memory) {
        i = 0;
    }
}

void Chip8::MemoryManager::place_program(const char* data, long size)
{
    int program_ptr = 0x200;
    for (size_t i = 0; i < size; i++) {
        m_memory[program_ptr] = data[i];
        program_ptr++;
    }
}

void Chip8::MemoryManager::dump()
{
    const size_t ROW_SIZE = 1 << 2;
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        if (i % ROW_SIZE == 0) {
            std::cout << '\n'
                      << std::flush;
        }
        std::cout << int_to_hex((int)m_memory[i]) << " ";
    }
}

unsigned short Chip8::MemoryManager::get_at_position(const u32 position)
{
    ensure_non_protected_access(position);
    ensure_non_protected_access(position + 1);

    return m_memory[position] << 8 | m_memory[position + 1];
}

void Chip8::MemoryManager::ensure_non_protected_access(const u32 position)
{
#ifdef USE_MEM_ASSERT
    ASSERT(position >= 0x200, "Access below 0x200 no allowed!");
    ASSERT(position < 0xF00, "Access above 0xF00 not allowed!");
#endif
}

bool Chip8::MemoryManager::is_program_end(u32 position)
{
    unsigned short op_code = get_at_position(position);
    return op_code == 0x0000;
}

void Chip8::MemoryManager::load_fontset()
{
    for (size_t i = 0; i < FONTSET_SIZE; i++) {
        m_memory[FONTSET_STAT_ADDRESS + i] = fontset[i];
    }
}
