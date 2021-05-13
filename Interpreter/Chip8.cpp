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
#include "Chip8.h"
#include <Entity.h>
#include <Graphics.h>
#include <Types.h>
#include <chrono>
#include <fstream>
#include <iostream>

Chip8::Chip8Application::Chip8Application(Graphics::Types::Size size)
    : Graphics::Window(size, Graphics::Types::Size(64, 32), "Chip8")
{
    m_memory_manager = std::make_shared<MemoryManager>();
    m_display = std::make_shared<DisplayBuffer>();
    m_cpu = std::make_unique<Cpu>(m_memory_manager, m_display);
}

void Chip8::Chip8Application::launch(const std::string& file)
{
    int video_pitch = sizeof(m_display->get_display_data()[0]) * m_display->get_width();
    load_program(file);
    bool quit = false;
    auto last_cycle = std::chrono::high_resolution_clock::now();
    while (!quit) {
        quit = process_input(m_cpu->get_keypad());
        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(now - last_cycle).count();
        if (dt > 1) {
            m_cpu->execute();
            update_texture(m_display->get_display_data(), video_pitch);
        }
    }
}

void Chip8::Chip8Application::load_program(const std::string& source_file)
{
    std::ifstream infile(source_file, std::ios::binary | std::ios::ate);
    if (infile.is_open()) {
        std::streampos size = infile.tellg();
        char* buffer = new char[size];

        infile.seekg(0, std::ios::beg);
        infile.read(buffer, size);
        infile.close();
        m_memory_manager->place_program(buffer, size);
        delete[] buffer;
    }
}
