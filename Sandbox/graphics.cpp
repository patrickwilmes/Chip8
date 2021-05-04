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
#include <Entity.h>
#include <Window.h>
#include <memory>

class MyRect : public Graphics::Entity {
protected:
    void draw_component(std::shared_ptr<Graphics::Painter> painter) override
    {
        Graphics::Types::Color color = { .r = 100, .g = 0, .b = 0, .a = 0 };
        Graphics::Types::Rectangle<int> rect(color, 200, 200, 100, 150);
        painter->draw_rect(rect, true);
    }
};

class MySquare : public Graphics::Entity {
protected:
    void draw_component(std::shared_ptr<Graphics::Painter> painter) override
    {
        Graphics::Types::Color color = { .r = 100, .g = 0, .b = 0, .a = 0 };
        Graphics::Types::Square<int> rect(color, 0, 0, 100);
        painter->draw_square(rect, true);
    }
};

int main()
{
    Graphics::Types::Size size(500, 500);
    Graphics::Window window(size, "Test Window");
    window.set_clear_color(100, 100, 0, 0);
    auto square = std::make_unique<MySquare>();
    auto rect = std::make_unique<MyRect>();
    window.register_entity(std::move(square));
    window.register_entity(std::move(rect));
    window.run();
    return 0;
}