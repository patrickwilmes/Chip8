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

Chip8::Cpu::Cpu(std::shared_ptr<MemoryManager> memory_manager, std::shared_ptr<Display> display)
    : m_memory_manager(std::move(memory_manager))
    , m_display(std::move(display))
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
    std::cout << "================REGISTER DUMP================" << '\n'
              << std::flush;
    dump();
    std::cout << "\n=============================================" << '\n'
              << std::flush;
    std::cout << "=================MEMORY DUMP=================" << '\n'
              << std::flush;
    m_memory_manager->dump();
    std::cout << "\n=============================================" << '\n'
              << std::flush;
    std::cout << "==================Display Dump=================" << '\n'
              << std::flush;
    m_display->dump();
    std::cout << "\n=============================================" << '\n'
              << std::flush;
}

void Chip8::Cpu::execute()
{
    bool should_quit = false;
    while (!should_quit) {
        unsigned short op_code = m_memory_manager->get_at_position(m_program_counter);
        std::cout << int_to_hex(op_code) << std::endl;
        switch (op_code & 0xF000) {
        case 0x0000: {
            unsigned short last_byte = op_code & 0x00FF;
            if (last_byte == 0xE0) {
                m_display->clear();
            } else if (last_byte == 0xEE) {
                //TODO: return from subroutine
            } else {
                //TODO: call machine code routine at address NNN.
            }
            break;
        }
        case 0x1000: {
            unsigned short target_address = op_code & 0x0FFF;
            m_program_counter_backup = m_program_counter;
            m_program_counter = target_address;
            break;
        }
        case 0x2000: {
            //TODO
            break;
        }
        case 0x3000: {
            //TODO
            break;
        }
        case 0x4000: {
            //TODO
            break;
        }
        case 0x5000: {
            //TODO
            break;
        }
        case 0x6000: {
            unsigned short target_register = (op_code & 0x0F00) >> 8;
            m_registers[target_register] = op_code & 0x00FF;
            break;
        }
        case 0x7000: {
            break;
        }
        case 0x8000: {
            //TODO:
            break;
        }
        case 0x9000: {
            //TODO:
            break;
        }
        case 0xA000: {
            //TODO
            break;
        }
        case 0xB000: {
            //TODO
            break;
        }
        case 0xC000: {
            //TODO
            break;
        }
        case 0xD000: {
            //TODO
            break;
        }
        case 0xE000: {
            //TODO
            break;
        }
        case 0xF000: {
            //TODO
            break;
        }
        }
        m_program_counter += 2;
        should_quit = m_memory_manager->is_program_end(m_program_counter);
    }
//    dump();
            core_dump();
}
