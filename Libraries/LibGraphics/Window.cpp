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
#include "Window.h"
#include <iostream>

Graphics::Window::Window(Graphics::Types::Size size, std::string title)
    : m_size(size)
{
    init();
    auto screen_info = initialize_screen_info();
    m_screen_width = screen_info.get_first();
    m_screen_height = screen_info.get_second();
    Graphics::Types::Point position(m_screen_width / 2 - size.get_first() / 2, m_screen_height / 2 - size.get_second() / 2);
    m_window = create_window(position, size, std::move(title));
    m_renderer = create_renderer(m_window);
    m_painter = initialize_painter(m_renderer, m_clear_color);
}

Graphics::Window::Window(Graphics::Types::Size size, Graphics::Types::Size texture_size, std::string title)
    : m_size(size)
{
    init();
    auto screen_info = initialize_screen_info();
    m_screen_width = screen_info.get_first();
    m_screen_height = screen_info.get_second();
    Graphics::Types::Point position(m_screen_width / 2 - size.get_first() / 2, m_screen_height / 2 - size.get_second() / 2);
    m_window = create_window(position, size, std::move(title));
    m_renderer = create_renderer(m_window);
    m_painter = initialize_painter(m_renderer, m_clear_color);
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texture_size.get_first(), texture_size.get_second());
}

Graphics::Window::~Window()
{
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Graphics::Window::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL init error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to init SDL");
    }
}

SDL_Window* Graphics::Window::create_window(Graphics::Types::Point position, Graphics::Types::Size size, std::string title)
{
    SDL_Window* win = SDL_CreateWindow(title.c_str(), position.get_first(), position.get_second(), size.get_first(), size.get_second(), SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        std::cerr << "SDL failed to create window: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to create sdl window!");
    }
    return win;
}

SDL_Renderer* Graphics::Window::create_renderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "SDL failed to create renderer: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to create sdl renderer!");
    }
    return renderer;
}

std::shared_ptr<Graphics::Painter> Graphics::Window::initialize_painter(SDL_Renderer* renderer, Graphics::Types::Color clear_color)
{
    auto painter = std::make_shared<Painter>(renderer, clear_color);
    return painter;
}

void Graphics::Window::run()
{
    SDL_Event e;
    bool should_quit = false;
    while (!should_quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                should_quit = true;
                break;
            }
        }

        SDL_SetRenderDrawColor(m_renderer, m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
        SDL_RenderClear(m_renderer);

        std::for_each(m_entities.begin(), m_entities.end(), [&](const std::shared_ptr<Entity>& entity) {
            entity->draw(m_painter);
        });

        if (!should_quit)
            should_quit = update_hook();
        update();

        SDL_RenderPresent(m_renderer);
    }
}

Common::Tuple<int> Graphics::Window::initialize_screen_info()
{
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    auto width = dm.w;
    auto height = dm.h;
    return Common::Tuple<int>(width, height);
}

void Graphics::Window::set_clear_color(Graphics::Types::Color color)
{
    m_clear_color = color;
}

void Graphics::Window::set_clear_color(int r, int g, int b, int a)
{
    set_clear_color({ .r = r, .g = g, .b = b, .a = a });
}

void Graphics::Window::register_entity(std::shared_ptr<Entity> entity)
{
    m_entities.emplace_back(std::move(entity));
}

void Graphics::Window::update()
{
    std::for_each(m_entities.begin(), m_entities.end(), [](const std::shared_ptr<Graphics::Entity>& entity) {
        entity->update();
    });
}

/**
 * update_hook is meant for windows that need some kind of special
 * processing hook that's not related to graphics.
 */
bool Graphics::Window::update_hook()
{
    return false;
}

int Graphics::Window::get_window_width()
{
    return m_size.get_first();
}

int Graphics::Window::get_window_height()
{
    return m_size.get_second();
}

void Graphics::Window::update_texture(void const* buffer, int pitch)
{
    SDL_UpdateTexture(m_texture, nullptr, buffer, pitch);
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

bool Graphics::Window::process_input(uint8_t *keys)
{
    bool quit = false;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
        {
            quit = true;
        } break;

        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            {
                quit = true;
            } break;

            case SDLK_x:
            {
                keys[0] = 1;
            } break;

            case SDLK_1:
            {
                keys[1] = 1;
            } break;

            case SDLK_2:
            {
                keys[2] = 1;
            } break;

            case SDLK_3:
            {
                keys[3] = 1;
            } break;

            case SDLK_q:
            {
                keys[4] = 1;
            } break;

            case SDLK_w:
            {
                keys[5] = 1;
            } break;

            case SDLK_e:
            {
                keys[6] = 1;
            } break;

            case SDLK_a:
            {
                keys[7] = 1;
            } break;

            case SDLK_s:
            {
                keys[8] = 1;
            } break;

            case SDLK_d:
            {
                keys[9] = 1;
            } break;

            case SDLK_z:
            {
                keys[0xA] = 1;
            } break;

            case SDLK_c:
            {
                keys[0xB] = 1;
            } break;

            case SDLK_4:
            {
                keys[0xC] = 1;
            } break;

            case SDLK_r:
            {
                keys[0xD] = 1;
            } break;

            case SDLK_f:
            {
                keys[0xE] = 1;
            } break;

            case SDLK_v:
            {
                keys[0xF] = 1;
            } break;
            }
        } break;

        case SDL_KEYUP:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_x:
            {
                keys[0] = 0;
            } break;

            case SDLK_1:
            {
                keys[1] = 0;
            } break;

            case SDLK_2:
            {
                keys[2] = 0;
            } break;

            case SDLK_3:
            {
                keys[3] = 0;
            } break;

            case SDLK_q:
            {
                keys[4] = 0;
            } break;

            case SDLK_w:
            {
                keys[5] = 0;
            } break;

            case SDLK_e:
            {
                keys[6] = 0;
            } break;

            case SDLK_a:
            {
                keys[7] = 0;
            } break;

            case SDLK_s:
            {
                keys[8] = 0;
            } break;

            case SDLK_d:
            {
                keys[9] = 0;
            } break;

            case SDLK_z:
            {
                keys[0xA] = 0;
            } break;

            case SDLK_c:
            {
                keys[0xB] = 0;
            } break;

            case SDLK_4:
            {
                keys[0xC] = 0;
            } break;

            case SDLK_r:
            {
                keys[0xD] = 0;
            } break;

            case SDLK_f:
            {
                keys[0xE] = 0;
            } break;

            case SDLK_v:
            {
                keys[0xF] = 0;
            } break;
            }
        } break;
        }
    }

    return quit;
}
