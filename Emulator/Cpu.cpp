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
#include "Cpu.h"
#include <Types.h>
#include <iostream>
#include <utility>

Chip8::Cpu::Cpu(std::shared_ptr<MemoryManager> memory_manager)
    : m_memory_manager(std::move(memory_manager))
{
}

void Chip8::Cpu::dump()
{
    const size_t ROW_SIZE = 1 << 2;
    for (size_t i = 0; i < 16; i++) {
        if (i % ROW_SIZE == 0) {
            std::cout << '\n'
                      << std::flush;
        }
        std::cout << Common::int_to_hex((int)m_registers[i]) << " ";
    }
    std::cout << '\n';
    std::cout << "ADDRESS REGISER: ";
    std::cout << Common::int_to_hex((int)m_address_register) << "\n";
}

void Chip8::Cpu::core_dump()
{
    std::cout << "================REGISTER DUMP================" << '\n' << std::flush;
    dump();
    std::cout << "\n=============================================" << '\n' << std::flush;
    std::cout << "=================MEMORY DUMP=================" << '\n' << std::flush;
    m_memory_manager->dump();
    std::cout << "\n=============================================" << '\n' << std::flush;
}
