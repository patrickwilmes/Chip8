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
#include <chrono>
#include <utility>

using namespace Common;

Chip8::Cpu::Cpu(std::shared_ptr<MemoryManager> memory_manager, std::shared_ptr<DisplayBuffer> display)
    : m_memory_manager(std::move(memory_manager))
    , m_display(std::move(display))
, m_random_generator(std::chrono::system_clock::now().time_since_epoch().count())
{
    m_random_byte = std::uniform_int_distribution<uint8_t>(0, 255U);

    table[0x0] = &Cpu::table_0;
    table[0x1] = &Cpu::opcode_1nnn;
    table[0x2] = &Cpu::opcode_2nnn;
    table[0x3] = &Cpu::opcode_3xkk;
    table[0x4] = &Cpu::opcode_4xkk;
    table[0x5] = &Cpu::opcode_5xy0;
    table[0x6] = &Cpu::opcode_6xkk;
    table[0x7] = &Cpu::opcode_7xkk;
    table[0x8] = &Cpu::table_8;
    table[0x9] = &Cpu::opcode_9xy0;
    table[0xA] = &Cpu::opcode_Annn;
    table[0xB] = &Cpu::opcode_Bnnn;
    table[0xC] = &Cpu::opcode_Cxkk;
    table[0xD] = &Cpu::opcode_Dxyn;
    table[0xE] = &Cpu::table_e;
    table[0xF] = &Cpu::table_f;

    table0[0x0] = &Cpu::opcode_00E0;
    table0[0xE] = &Cpu::opcode_00EE;

    table8[0x0] = &Cpu::opcode_8xy0;
    table8[0x1] = &Cpu::opcode_8xy1;
    table8[0x2] = &Cpu::opcode_8xy2;
    table8[0x3] = &Cpu::opcode_8xy3;
    table8[0x4] = &Cpu::opcode_8xy4;
    table8[0x5] = &Cpu::opcode_8xy5;
    table8[0x6] = &Cpu::opcode_8xy6;
    table8[0x7] = &Cpu::opcode_8xy7;
    table8[0xE] = &Cpu::opcode_8xyE;

    tableE[0x1] = &Cpu::opcode_ExA1;
    tableE[0xE] = &Cpu::opcode_Ex9E;

    tableF[0x07] = &Cpu::opcode_Fx07;
    tableF[0x0A] = &Cpu::opcode_Fx0A;
    tableF[0x15] = &Cpu::opcode_Fx15;
    tableF[0x18] = &Cpu::opcode_Fx18;
    tableF[0x1E] = &Cpu::opcode_Fx1E;
    tableF[0x29] = &Cpu::opcode_Fx29;
    tableF[0x33] = &Cpu::opcode_Fx33;
    tableF[0x55] = &Cpu::opcode_Fx55;
    tableF[0x65] = &Cpu::opcode_Fx65;
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
    std::cout << "==================DisplayBuffer Dump=================" << '\n'
              << std::flush;
    m_display->dump();
    std::cout << "\n=============================================" << '\n'
              << std::flush;
}

void Chip8::Cpu::execute()
{
    m_opcode = m_memory_manager->get_at_position(m_program_counter);
    m_program_counter += 2;
    ((*this).*(table[(m_opcode & 0xF000u) >> 12u]))();
}

void Chip8::Cpu::opcode_none()
{
}

void Chip8::Cpu::table_0()
{
    ((*this).*(table0[m_opcode & 0x000Fu]))();
}

void Chip8::Cpu::table_8()
{
    ((*this).*(table8[m_opcode & 0x000Fu]))();
}

void Chip8::Cpu::table_e()
{
    ((*this).*(tableE[m_opcode & 0x000Fu]))();
}

void Chip8::Cpu::table_f()
{
    ((*this).*(tableF[m_opcode & 0x00FFu]))();
}

void Chip8::Cpu::opcode_00E0()
{
    m_display->clear();
}

void Chip8::Cpu::opcode_00EE()
{
    --m_sp;
    m_program_counter = m_stack[m_sp];
}

void Chip8::Cpu::opcode_1nnn()
{
    uint16_t address = m_opcode & 0x0FFFu;
    m_program_counter = address;
}

void Chip8::Cpu::opcode_2nnn()
{
    uint16_t address = m_opcode & 0xFFFu;
    m_stack[m_sp] = m_program_counter;
    ++m_sp;
    m_program_counter = address;
}

void Chip8::Cpu::opcode_3xkk()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t byte = m_opcode & 0x00FFu;
    if (m_registers[vx] == byte) {
        m_program_counter += 2;
    }
}

void Chip8::Cpu::opcode_4xkk()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t byte = m_opcode & 0x00FFu;

    if (m_registers[vx] != byte) {
        m_program_counter += 2;
    }
}

void Chip8::Cpu::opcode_5xy0()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t vy = (m_opcode & 0x00F0u) >> 4u;

    if (m_registers[vx] == m_registers[vy]) {
        m_program_counter += 2;
    }
}

void Chip8::Cpu::opcode_6xkk()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t byte = m_opcode & 0x00FFu;

    m_registers[vx] = byte;
}

void Chip8::Cpu::opcode_7xkk()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t byte = m_opcode & 0x00FFu;

    m_registers[vx] += byte;
}

void Chip8::Cpu::opcode_8xy0()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t vy = (m_opcode & 0x00F0u) >> 4u;

    m_registers[vx] = m_registers[vy];
}

void Chip8::Cpu::opcode_8xy1()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t vy = (m_opcode & 0x00F0u) >> 4u;

    m_registers[vx] |= m_registers[vy];
}

void Chip8::Cpu::opcode_8xy2()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t vy = (m_opcode & 0x00F0u) >> 4u;

    m_registers[vx] &= m_registers[vy];
}

void Chip8::Cpu::opcode_8xy3()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t vy = (m_opcode & 0x00F0u) >> 4u;

    m_registers[vx] ^= m_registers[vy];
}

void Chip8::Cpu::opcode_8xy4()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t vy = (m_opcode & 0x00F0u) >> 4u;

    uint16_t sum = m_registers[vx] + m_registers[vy];

    if (sum > 255u) {
        m_registers[0xF] = 1;
    } else {
        m_registers[0xF] = 0;
    }

    m_registers[vx] = sum & 0xFFu;
}

void Chip8::Cpu::opcode_8xy5()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t vy = (m_opcode & 0x00F0u) >> 4u;

    if (m_registers[vx] > m_registers[vy]) {
        m_registers[0xF] = 1;
    } else {
        m_registers[0xF] = 0;
    }

    m_registers[vx] -= m_registers[vy];
}

void Chip8::Cpu::opcode_8xy6()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    m_registers[0xF] = (m_registers[vx] & 0x1u);

    m_registers[vx] >>= 1;
}

void Chip8::Cpu::opcode_8xy7()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t vy = (m_opcode & 0x00F0u) >> 4u;

    if (m_registers[vy] > m_registers[vx]) {
        m_registers[0xF] = 1;
    } else {
        m_registers[0xF] = 0;
    }

    m_registers[vx] = m_registers[vy] - m_registers[vx];
}

void Chip8::Cpu::opcode_8xyE()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    m_registers[0xF] = (m_registers[vx] & 0x80u) >> 7u;

    m_registers[vx] <<= 1;
}

void Chip8::Cpu::opcode_9xy0()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t vy = (m_opcode & 0x00F0u) >> 4u;

    if (m_registers[vx] != m_registers[vy]) {
        m_program_counter += 2;
    }
}

void Chip8::Cpu::opcode_Annn()
{
    uint16_t address = m_opcode & 0x0FFFu;

    m_address_register = address;
}

void Chip8::Cpu::opcode_Bnnn()
{
    uint16_t address = m_opcode & 0x0FFFu;

    m_program_counter = m_registers[0] + address;
}

void Chip8::Cpu::opcode_Cxkk()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t byte = m_opcode & 0x00FFu;

    m_registers[vx] = m_random_byte(m_random_generator) & byte;
}

void Chip8::Cpu::opcode_Dxyn()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t vy = (m_opcode & 0x00F0u) >> 4u;
    uint8_t height = m_opcode & 0x000Fu;

    uint8_t x_pos = m_registers[vx] % 64;
    uint8_t y_pos = m_registers[vy] % 32;

    m_registers[0xF] = 0;

    for (unsigned int row = 0; row < height; ++row) {
        uint8_t sprite_byte = m_memory_manager->get_value(m_address_register + row);
        for (unsigned int col = 0; col < 8; ++col) {
            uint8_t sprite_pixel = sprite_byte & (0x80u >> col);
            uint32_t * screen_pixel = &m_display->get_display_data()[(y_pos + row) * 64 + (x_pos + col)];
            if (sprite_pixel) {
                if (*screen_pixel == 0xFFFFFFFF)
                {
                    m_registers[0xF] = 1;
                }

                *screen_pixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void Chip8::Cpu::opcode_Ex9E()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    uint8_t key = m_registers[vx];

    if (m_keypad[key]) {
        m_program_counter += 2;
    }
}

void Chip8::Cpu::opcode_ExA1()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    uint8_t key = m_registers[vx];

    if (!m_keypad[key]) {
        m_program_counter += 2;
    }
}

void Chip8::Cpu::opcode_Fx07()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    m_registers[vx] = m_delay_timer;
}

void Chip8::Cpu::opcode_Fx0A()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    if (m_keypad[0])
    {
        m_registers[vx] = 0;
    }
    else if (m_keypad[1])
    {
        m_registers[vx] = 1;
    }
    else if (m_keypad[2])
    {
        m_registers[vx] = 2;
    }
    else if (m_keypad[3])
    {
        m_registers[vx] = 3;
    }
    else if (m_keypad[4])
    {
        m_registers[vx] = 4;
    }
    else if (m_keypad[5])
    {
        m_registers[vx] = 5;
    }
    else if (m_keypad[6])
    {
        m_registers[vx] = 6;
    }
    else if (m_keypad[7])
    {
        m_registers[vx] = 7;
    }
    else if (m_keypad[8])
    {
        m_registers[vx] = 8;
    }
    else if (m_keypad[9])
    {
        m_registers[vx] = 9;
    }
    else if (m_keypad[10])
    {
        m_registers[vx] = 10;
    }
    else if (m_keypad[11])
    {
        m_registers[vx] = 11;
    }
    else if (m_keypad[12])
    {
        m_registers[vx] = 12;
    }
    else if (m_keypad[13])
    {
        m_registers[vx] = 13;
    }
    else if (m_keypad[14])
    {
        m_registers[vx] = 14;
    }
    else if (m_keypad[15])
    {
        m_registers[vx] = 15;
    }
    else
    {
        m_program_counter -= 2;
    }
}

void Chip8::Cpu::opcode_Fx15()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    m_delay_timer = m_registers[vx];
}

void Chip8::Cpu::opcode_Fx18()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    m_sound_timer = m_registers[vx];
}

void Chip8::Cpu::opcode_Fx1E()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    m_address_register += m_registers[vx];
}

void Chip8::Cpu::opcode_Fx29()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t digit = m_registers[vx];

    m_address_register = 0x50 + (5 * digit);
}

void Chip8::Cpu::opcode_Fx33()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;
    uint8_t value = m_registers[vx];

    m_memory_manager->set_value(m_address_register + 2, value % 10);
    value /= 10;

    m_memory_manager->set_value(m_address_register + 1, value % 10);
    value /= 10;

    m_memory_manager->set_value(m_address_register, value % 10);
}

void Chip8::Cpu::opcode_Fx55()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    for (uint8_t i = 0; i <= vx; ++i) {
        m_memory_manager->set_value(m_address_register + i, m_registers[i]);
    }
}

void Chip8::Cpu::opcode_Fx65()
{
    uint8_t vx = (m_opcode & 0x0F00u) >> 8u;

    for (uint8_t i = 0; i <= vx; ++i) {
        m_registers[i] = m_memory_manager->get_value(m_address_register + i);
    }
}

uint8_t* Chip8::Cpu::get_keypad()
{
    return m_keypad;
}
