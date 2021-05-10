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
#include "Display.h"
#include <Types.h>
#include <cstddef>
#include <iostream>

void Chip8::Display::apply_display_data(const unsigned short* new_display_data)
{
    for (size_t i = 0; i < 32 * 64; i++) {
        unsigned short current_value = m_display_data[i];
        unsigned short new_value = new_display_data[i];

        unsigned short ored = current_value | new_value;
        unsigned short neg_ored = ~current_value | ~new_value;
        m_display_data[i] = ored & neg_ored;
    }
}

void Chip8::Display::clear()
{
    for (auto& data : m_display_data) {
        data = 0;
    }
}

void Chip8::Display::dump()
{
    const size_t ROW_SIZE = 1 << 6;
    for (size_t i = 0; i < 32 * 64; i++) {
        if (i % ROW_SIZE == 0) {
            std::cout << '\n'
                      << std::flush;
        }
        std::cout << Common::int_to_hex((int)m_display_data[i]) << " ";
    }
}

int Chip8::Display::get_width()
{
    return DISPLAY_WIDTH;
}

int Chip8::Display::get_height()
{
    return DISPLAY_HEIGHT;
}

unsigned short* Chip8::Display::get_display_data()
{
    return m_display_data;
}
