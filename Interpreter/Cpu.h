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
#pragma once
#include "DisplayBuffer.h"
#include "Memory.h"
#include <memory>
#include <random>

namespace Chip8 {
    const unsigned int KEY_COUNT = 16;

    class Cpu final {
    public:
        Cpu(std::shared_ptr<MemoryManager> memory_manager, std::shared_ptr<DisplayBuffer> display);
        void dump();
        void core_dump();
        void execute();
        uint8_t * get_keypad();

    private:
        void table_0();
        void table_8();
        void table_e();
        void table_f();

        void opcode_none();
        void opcode_00E0();
        void opcode_00EE();
        void opcode_1nnn();
        void opcode_2nnn();
        void opcode_3xkk();
        void opcode_4xkk();
        void opcode_5xy0();
        void opcode_6xkk();
        void opcode_7xkk();
        void opcode_8xy0();
        void opcode_8xy1();
        void opcode_8xy2();
        void opcode_8xy3();
        void opcode_8xy4();
        void opcode_8xy5();
        void opcode_8xy6();
        void opcode_8xy7();
        void opcode_8xyE();
        void opcode_9xy0();
        void opcode_Annn();
        void opcode_Bnnn();
        void opcode_Cxkk();
        void opcode_Dxyn();
        void opcode_Ex9E();
        void opcode_ExA1();
        void opcode_Fx07();
        void opcode_Fx0A();
        void opcode_Fx15();
        void opcode_Fx18();
        void opcode_Fx1E();
        void opcode_Fx29();
        void opcode_Fx33();
        void opcode_Fx55();
        void opcode_Fx65();

    private:
        std::shared_ptr<MemoryManager> m_memory_manager;
        std::shared_ptr<DisplayBuffer> m_display;

        uint8_t m_delay_timer{};
        uint8_t m_sound_timer{};

        std::default_random_engine m_random_generator;
        std::uniform_int_distribution<uint8_t> m_random_byte;
        uint8_t m_registers[16] {};
        uint16_t m_address_register {};
        uint16_t m_program_counter = 0x200;
        uint16_t m_stack[16] {};
        uint8_t m_sp{};
        uint16_t m_opcode{};

        uint8_t m_keypad[KEY_COUNT]{};

        typedef void (Cpu::*OpCodeFunc)();
        OpCodeFunc table[0xF + 1]{&Cpu::opcode_none};
        OpCodeFunc table0[0xE + 1]{&Cpu::opcode_none};
        OpCodeFunc table8[0xE + 1]{&Cpu::opcode_none};
        OpCodeFunc tableE[0xE + 1]{&Cpu::opcode_none};
        OpCodeFunc tableF[0x65 + 1]{&Cpu::opcode_none};
    };
}