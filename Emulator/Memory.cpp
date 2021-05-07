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
#include <cstddef>
#include <iostream>
#include <Assert.h>

using namespace Common;

Chip8::MemoryManager::MemoryManager()
{
    reset_memory();
}

void Chip8::MemoryManager::reset_memory()
{
    for (char& i : m_memory) {
        i = 0;
    }
}

void Chip8::MemoryManager::place_program(char* data, int size)
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

char Chip8::MemoryManager::get_at_position(const u32 position)
{
    ensure_non_protected_access(position);
    return m_memory[position];
}

void Chip8::MemoryManager::ensure_non_protected_access(const u32 position)
{
    ASSERT(position >= 0x200, "Access below 0x200 no allowed!");
    ASSERT(position < 0xF00, "Access above 0xF00 not allowed!");
}
