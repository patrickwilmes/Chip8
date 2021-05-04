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
#include <Types.h>

namespace Graphics::Types {
    typedef Common::Tuple<int> Point;
    typedef Common::Tuple<int> Size;

    typedef struct {
        int r, g, b, a;
    } Color;

    template<typename Type>
    class Rectangle {
    public:
        Rectangle(Color color, Type x, Type y, Type width, Type height)
            : m_color(color)
            , m_x(x)
            , m_y(y)
            , m_width(width)
            , m_height(height)
        {
        }

        Type get_x()
        {
            return m_x;
        }
        Type get_y()
        {
            return m_y;
        }
        Type get_width()
        {
            return m_width;
        }
        Type get_height()
        {
            return m_height;
        }
        int get_r()
        {
            return m_color.r;
        }
        int get_g()
        {
            return m_color.g;
        }
        int get_b()
        {
            return m_color.b;
        }
        int get_a()
        {
            return m_color.a;
        }

    private:
        Color m_color;
        Type m_x, m_y, m_width, m_height;
    };

    template<typename Type>
    class Square : public Rectangle<Type> {
    public:
        Square(Color color, Type x, Type y, Type width)
            : Rectangle<Type>(color, x, y, width, width)
        {
        }
    };

}