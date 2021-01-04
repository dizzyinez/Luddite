#pragma once
#include <string>
// #include <ft2build.h>
#include "data/FontAllocator.hpp"
#include "entt.hpp"

enum class text_align : uint8_t
{
        LEFT,
        RIGHT,
        CENTER,
        CENTER_ON_ZERO
};

enum class text_align_vertical : uint8_t
{
        TOP,
        BOTTOM,
        CENTER
};

struct C_Text
{
        C_Text() = default;
        C_Text(const std::string& text_) : text(text_) {}
        C_Text(char* text_) : text(text_) {}
        std::string text = "";
        std::vector<entt::entity> character_entities;
        float scale = 1.0f;
        float max_length;
        text_align align = text_align::LEFT;
        text_align_vertical align_vertical = text_align_vertical::TOP;
        bool dirty = true;
        bool recalculate_position_flag = true;
        C_Text& set_alignment(text_align a) {align = a; recalculate_position_flag = true; return *this;}
        C_Text& set_vertical_alignment(text_align_vertical av) {align_vertical = av; recalculate_position_flag = true; return *this;}
        C_Text& set_scale(float s) {scale = s; recalculate_position_flag = true; return *this;}
        C_Text& set_pixel_scale(float s) {scale = s / 128.0f; recalculate_position_flag = true; return *this;}
};

// struct C_Character
// {
// };