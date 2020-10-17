#pragma once
#include <string>
#include <ft2build.h>

struct C_Text
{
        C_Text() = default;
        C_Text(const std::string& text_) : text(text_) {}
        C_Text(char* text_) : text(text_) {}
        std::string text = "";
};
