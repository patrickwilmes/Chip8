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
#include <Print.h>
#include <Types.h>
#include <cmath>
#include <iostream>
#include <utility>

using namespace Common;

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

bool Chip8::Cpu::execute()
{
    bool should_quit;
    unsigned short op_code = m_memory_manager->get_at_position(m_program_counter);
    std::cout << int_to_hex(op_code) << std::endl;
    bool should_increment_pc = true;
    switch (op_code & 0xF000) {
    case 0x0000: {
        unsigned short last_byte = op_code & 0x00FF;
        if (last_byte == 0xE0) {
            msg("clearing screen\n");
            m_display->clear();
        } else if (last_byte == 0xEE) {
            msg("Returning from subroutine\n");
            m_program_counter = m_program_counter_backup;
        } else {
            msg("Calling subroutine\n");
            m_program_counter_backup = m_program_counter;
            auto new_address = op_code & 0x0FFF;
            m_program_counter = new_address;
            should_increment_pc = false;
        }
        break;
    }
    case 0x1000: {
        unsigned short target_address = op_code & 0x0FFF;
        m_program_counter_backup = m_program_counter;
        m_program_counter = target_address;
        msg("moving to a different address. from: ", int_to_hex(m_program_counter_backup), " to: ", int_to_hex(m_program_counter));
        break;
    }
    case 0x2000: {
        msg("Calling subroutine 0x2000\n");
        auto new_address = op_code & 0x0FFF;
        m_program_counter_backup = m_program_counter;
        m_program_counter = new_address;
        should_increment_pc = false;
        break;
    }
    case 0x3000: {
        msg("Checking 0x3000 if should skip next instruction\n");
        auto target_register = (op_code & 0x0F00) >> 8;
        auto target_value = op_code & 0x00FF;
        if (m_registers[target_register] == target_value) {
            msg("0x3000 is true skipping next instruction\n");
            m_program_counter += 2;
        }
        break;
    }
    case 0x4000: {
        msg("Checking 0x4000 if should skip next instruction\n");
        auto target_register = (op_code & 0x0F00) >> 8;
        auto target_value = op_code & 0x00FF;
        if (m_registers[target_register] != target_value) {
            msg("0x4000 is true skipping next instruction\n");
            m_program_counter += 2;
        }
        break;
    }
    case 0x5000: {
        msg("Checking 0x5000 if should skip next instruction\n");
        auto first_target_register = (op_code & 0x0F00) >> 8;
        auto second_target_register = (op_code & 0x00F0) >> 8;
        if (m_registers[first_target_register] == m_registers[second_target_register]) {
            msg("0x5000 is true skipping next instruction\n");
            m_program_counter += 2;
        }
        break;
    }
    case 0x6000: {
        unsigned short target_register = (op_code & 0x0F00) >> 8;
        m_registers[target_register] = op_code & 0x00FF;
        msg("pushing value into register. register V", int_to_hex(target_register), " value: ", op_code & 0x00FF);
        break;
    }
    case 0x7000: {
        auto target_register = (op_code & 0x0F00) >> 8;
        auto value = op_code & 0x00FF;
        msg("Adding value ", value, " to register ", target_register);
        m_registers[target_register] += value;
        break;
    }
    case 0x8000: {
        msg("Accessing 0x8000\n");
        auto mode = op_code & 0x000F;
        msg("0x8000 having mode ", int_to_hex(mode));
        switch (mode) {
        case 0x0: {
            msg("Mode 0x8000 case 0\n");
            auto first_register = (op_code & 0x0F00) >> 8;
            auto second_register = (op_code & 0x00F0) >> 8;
            m_registers[first_register] = m_registers[second_register];
            break;
        }
        case 0x1: {
            msg("Mode 0x8000 case 1\n");
            auto first_register = (op_code & 0x0F00) >> 8;
            auto second_register = (op_code & 0x00F0) >> 8;
            m_registers[first_register] = m_registers[first_register] | m_registers[second_register];
            break;
        }
        case 0x2: {
            msg("Mode 0x8000 case 2\n");
            auto first_register = (op_code & 0x0F00) >> 8;
            auto second_register = (op_code & 0x00F0) >> 8;
            m_registers[first_register] = m_registers[first_register] & m_registers[second_register];
            break;
        }
        case 0x3: {
            msg("Mode 0x8000 case 3\n");
            auto first_register = (op_code & 0x0F00) >> 8;
            auto second_register = (op_code & 0x00F0) >> 8;
            m_registers[first_register] = m_registers[first_register] ^ m_registers[second_register];
            break;
        }
        case 0x4: {
            msg("Mode 0x8000 case 4\n");
            auto first_register = (op_code & 0x0F00) >> 8;
            auto second_register = (op_code & 0x00F0) >> 8;
            auto first_value = m_registers[first_register];
            auto second_value = m_registers[second_register];
            auto result = first_value + second_value;
            if (abs(result) > 9)
                m_registers[VF] = 1;
            else
                m_registers[VF] = 0;
            m_registers[first_register] = result;
            break;
        }
        case 0x5: {
            msg("Mode 0x8000 case 5\n");
            auto first_register = (op_code & 0x0F00) >> 8;
            auto second_register = (op_code & 0x00F0) >> 8;
            auto first_value = m_registers[first_register];
            auto second_value = m_registers[second_register];
            auto result = first_value - second_value;
            if (abs(result) > 9)
                m_registers[VF] = 1;
            else
                m_registers[VF] = 0;
            m_registers[first_register] = result;
            break;
        }
        case 0x6: {
            msg("Mode 0x8000 case 6\n");
            auto first_register = (op_code & 0x0F00) >> 8;
            auto value = m_registers[first_register];
            auto least_significant_bit = value & 1;
            m_registers[VF] = least_significant_bit;
            m_registers[first_register] = m_registers[first_register] >> 1;
            break;
        }
        case 0x7: {
            msg("Mode 0x8000 case 7\n");
            auto first_register = (op_code & 0x0F00) >> 8;
            auto second_register = (op_code & 0x00F0) >> 8;
            auto value = m_registers[second_register] - m_registers[first_register];
            if (abs(value) > 9)
                m_registers[VF] = 1;
            else
                m_registers[VF] = 0;
            m_registers[first_register] = value;
            break;
        }
        case 0xE: {
            auto msb = [](int n) {
                if (n == 0)
                    return 0;

                int msb = 0;
                n = n / 2;
                while (n != 0) {
                    n = n / 2;
                    msb++;
                }

                return (1 << msb);
            };
            msg("Mode 0x8000 case E\n");
            auto first_register = (op_code & 0x0F00) >> 8;
            auto value = m_registers[first_register];
            m_registers[VF] = msb(value);
            m_registers[first_register] = m_registers[first_register] << 1;
            break;
        }
        }
        break;
    }
    case 0x9000: {
        msg("0x9000 checking if next instruction should be skipped\n");
        auto first_register = (op_code & 0x0F00) >> 8;
        auto second_register = (op_code & 0x0F00) >> 8;
        if (m_registers[first_register] != m_registers[second_register]) {
            m_program_counter_backup = m_program_counter;
            m_program_counter += 2;
        }
        break;
    }
    case 0xA000: {
        msg("0xA000 setting I register\n");
        auto address = op_code & 0x0FFF;
        m_address_register = address;
        break;
    }
    case 0xB000: {
        msg("0xB000 jumping to new location\n");
        auto address = m_registers[V0] + op_code & 0x0FFF;
        m_program_counter_backup = m_program_counter;
        m_program_counter = address;
        break;
    }
    case 0xC000: {
        msg("0xC000 applying rand\n");
        auto target_register = (op_code & 0x0F00) >> 8;
        auto value = op_code & 0x00FF;
        auto rand = std::rand() % 255;
        m_registers[target_register] = value & rand;
        break;
    }
    case 0xD000: {
        //FIXME: this might work but is not correctly implemented
        auto first_register = (op_code & 0x0F00) >> 8;
        auto second_register = (op_code & 0x00F0) >> 8;
        auto value = op_code & 0x000F;
        m_display->set_pixel(m_registers[first_register], m_registers[second_register], value);
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
    default: {
        std::cerr << "unknown opcode: " << int_to_hex(op_code) << '\n'
                  << std::flush;
        core_dump();
        break;
    }
    }
    if (should_increment_pc)
        m_program_counter += 2;
    should_quit = m_memory_manager->is_program_end(m_program_counter);
    return should_quit;
}
