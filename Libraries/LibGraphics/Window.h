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
#include "Common.h"
#include "Entity.h"
#include "Graphics.h"
#include <SDL2/SDL.h>
#include <Types.h>
#include <memory>
#include <string>
#include <vector>

namespace Graphics {
    using namespace Common;
    class Window {
    public:
        Window(Graphics::Types::Size size, std::string title);
        Window(Graphics::Types::Point position, Graphics::Types::Size size, std::string title);
        ~Window();
        void run();
        void set_clear_color(Graphics::Types::Color color);
        void set_clear_color(int r, int g, int b, int a);
        void register_entity(std::unique_ptr<Entity> entity);

    protected:
        std::vector<std::unique_ptr<Graphics::Entity>> m_entities;
        virtual void update_hook();

    private:
        static void init();
        static SDL_Window* create_window(Graphics::Types::Point position, Graphics::Types::Size size, std::string title);
        static SDL_Renderer* create_renderer(SDL_Window* window);
        static Tuple<int> initialize_screen_info();
        static std::shared_ptr<Painter> initialize_painter(SDL_Renderer* renderer, Graphics::Types::Color clear_color);
        void update();

    private:
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;
        int m_screen_width;
        int m_screen_height;
        Graphics::Types::Color m_clear_color = { .r = 0, .g = 0, .b = 0, .a = 0 };
        std::shared_ptr<Painter> m_painter = nullptr;
    };
}
