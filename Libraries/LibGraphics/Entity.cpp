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
#include "Entity.h"

Graphics::Entity::Entity(int x, int y, int width, int height, Graphics::Types::Color color)
    : m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_color(color)
{
}

void Graphics::Entity::draw(const std::shared_ptr<Painter>& painter)
{
    draw_component(painter);
}

int Graphics::Entity::get_x() const
{
    return m_x;
}

int Graphics::Entity::get_y() const
{
    return m_y;
}

int Graphics::Entity::get_width() const
{
    return m_width;
}

int Graphics::Entity::get_height() const
{
    return m_height;
}

void Graphics::Entity::set_x(int x)
{
    m_x = x;
}

void Graphics::Entity::set_y(int y)
{
    m_y = y;
}

void Graphics::Entity::set_width(int width)
{
    m_width = width;
}

void Graphics::Entity::set_height(int height)
{
    m_height = height;
}

void Graphics::Entity::set_color(Graphics::Types::Color color)
{
    m_color = color;
}
