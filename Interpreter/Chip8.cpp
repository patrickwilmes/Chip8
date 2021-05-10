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

constexpr Graphics::Types::Color PIXEL_ON_COLOR = { .r = 255, .g = 255, .b = 255, .a = 0 };
constexpr Graphics::Types::Color PIXEL_OFF_COLOR = { .r = 0, .g = 0, .b = 0, .a = 0 };

class Pixel : public Graphics::Entity {
public:
    explicit Pixel(Graphics::Types::Rectangle<int> initial_position)
        : m_rect(initial_position)
    {
    }
    void update() override
    {
    }
    void toggle_off()
    {
        m_current_color = PIXEL_OFF_COLOR;
    }
    void toggle_on()
    {
        m_current_color = PIXEL_ON_COLOR;
    }

protected:
    void draw_component(std::shared_ptr<Graphics::Painter> painter) override
    {
        painter->draw_rect(m_rect);
    }

private:
    Graphics::Types::Rectangle<int> m_rect;
    Graphics::Types::Color m_current_color;
};

Chip8::Interpreter::Interpreter()
{
    m_memory_manager = std::make_shared<MemoryManager>();
    m_display = std::make_shared<Display>();
    m_cpu = std::make_unique<Cpu>(m_memory_manager, m_display);
}

void Chip8::Interpreter::emulate(const std::string& source_file)
{
    load_program(source_file);
}

int Chip8::Interpreter::get_display_width()
{
    return m_display->get_width();
}

int Chip8::Interpreter::get_display_height()
{
    return m_display->get_height();
}

unsigned short* Chip8::Interpreter::get_display_data()
{
    return m_display->get_display_data();
}

void Chip8::Interpreter::load_program(const std::string& source_file)
{
    unsigned char buffer[1 << 12];
    FILE* file = fopen(source_file.c_str(), "rb");
    int bytes_read = fread(buffer, sizeof(char), 1 << 12, file);
    m_memory_manager->place_program(buffer, bytes_read);
}

bool Chip8::Interpreter::execute_next_cycle()
{
    return m_cpu->execute();
}

Chip8::Chip8Application::Chip8Application(Graphics::Types::Size size)
    : Graphics::Window(size, "Chip8")
{
}

void Chip8::Chip8Application::launch(const std::string& file)
{
    m_interpreter.emulate(file);
    auto width = m_interpreter.get_display_width();
    auto height = m_interpreter.get_display_height();
    auto window_width = get_window_width();
    auto window_height = get_window_height();
    auto cell_height = window_height / height;
    auto cell_width = window_width / width;
    int current_row = -1;
    int current_col = 0;
    for (size_t i = 0; i < width * height; i++) {
        if (i % width == 0) {
            current_col = 0;
            current_row++;
        } else {
            current_col += cell_width;
        }
        Graphics::Types::Rectangle<int> rect(PIXEL_OFF_COLOR, current_col, current_row * cell_height, cell_width, cell_height);
        auto entity = std::make_unique<Pixel>(rect);
        register_entity(std::move(entity));
    }
    run();
}

bool Chip8::Chip8Application::update_hook()
{
    bool should_quit = m_interpreter.execute_next_cycle();
    int display_width = m_interpreter.get_display_width();
    int display_height = m_interpreter.get_display_height();
    unsigned short* display_data = m_interpreter.get_display_data();
    //TODO: this is the place to hookup the display rendering.
    return should_quit;
}
