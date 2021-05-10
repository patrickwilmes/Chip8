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
#include <cstdio>
#include <iostream>

class Sprite : public Graphics::Entity {
public:
    Sprite(Graphics::Types::Point initial_position, int height, int cell_width)
        : m_initial_position(initial_position)
        , m_height(height == 0 ? 1 : height)
        , m_cell_width(cell_width == 0 ? 10 : cell_width)
    {
    }

    void update() override
    {
    }

protected:
    void draw_component(std::shared_ptr<Graphics::Painter> painter) override
    {
        Graphics::Types::Rectangle<int> rect(
            COLOR_WHITE,
            m_initial_position.get_first(),
            m_initial_position.get_second(),
            8 * m_cell_width,
            m_height);
        painter->draw_rect(rect);
    }

private:
    Graphics::Types::Point m_initial_position;
    int m_height;
    int m_cell_width;
    static constexpr Graphics::Types::Color COLOR_WHITE = { .r = 255, .g = 255, .b = 255, .a = 0 };
};

Chip8::Chip8Application::Chip8Application(Graphics::Types::Size size)
    : Graphics::Window(size, "Chip8")
{
    m_memory_manager = std::make_shared<MemoryManager>();
    m_display = std::make_shared<Display>();
    m_cpu = std::make_unique<Cpu>(m_memory_manager, m_display);
}

void Chip8::Chip8Application::launch(const std::string& file)
{
    load_program(file);
    run();
}

bool Chip8::Chip8Application::update_hook()
{
    bool should_quit = m_cpu->execute();
    return should_quit;
}

void Chip8::Chip8Application::load_program(const std::string& source_file)
{
    unsigned char buffer[1 << 12];
    FILE* file = fopen(source_file.c_str(), "rb");
    int bytes_read = fread(buffer, sizeof(char), 1 << 12, file);
    m_memory_manager->place_program(buffer, bytes_read);
}
