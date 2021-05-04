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
#include "Path.h"
#include "User.h"
#include <Print.h>

std::string Common::resolve_relative_path(std::string path)
{
    char first_char = path[0];
    // if we start not with ~ or we start with / then we are not relative
    if (first_char != '~' && first_char != '/') {
        return path;
    }
    if (first_char == '~') {
        auto user = Common::get_user_info();
        auto home_dir = user.home_dir;
        return path.replace(0, 1, user.home_dir);
    } else {
    }
    return "";
}

std::string Common::concat_path(std::string path_one, std::string path_two)
{
    char last_char_path_one = path_one[path_one.length() - 1];
    char first_char_path_two = path_two[0];
    if ((last_char_path_one == '/' && first_char_path_two != '/') || (last_char_path_one != '/' && first_char_path_two == '/')) {
        return path_one + path_two;
    }
    if (last_char_path_one == '/') {
        return path_one.erase(path_one.length() - 1) + path_two;
    }
    return path_one + "/" + path_two;
}
