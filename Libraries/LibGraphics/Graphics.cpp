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
#include "Graphics.h"

Graphics::Painter::Painter(SDL_Renderer* renderer, Graphics::Types::Color clear_color)
    : m_clear_color(clear_color)
    , m_renderer(renderer)
{
}

void Graphics::Painter::draw_rect(Graphics::Types::Rectangle<int>& rect, bool fill)
{
    SDL_Rect sdl_rect = {
        .x = rect.get_x(),
        .y = rect.get_y(),
        .w = rect.get_width(),
        .h = rect.get_height()
    };
    SDL_SetRenderDrawColor(m_renderer, rect.get_r(), rect.get_g(), rect.get_b(), rect.get_a());
    if (fill)
        SDL_RenderFillRect(m_renderer, &sdl_rect);
    else
        SDL_RenderDrawRect(m_renderer, &sdl_rect);
}

void Graphics::Painter::reset_draw_color()
{
    SDL_SetRenderDrawColor(m_renderer, m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
}

void Graphics::Painter::draw_square(Graphics::Types::Square<int>& rect, bool fill)
{
    draw_rect(rect, fill);
}
