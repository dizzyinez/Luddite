#pragma once
#include <string>
// #include <ft2build.h>
#include <memory>
#include "data/FontAllocator.hpp"
#include "entt.hpp"
#include "core/basic_includes.hpp"

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
        C_Text(const String& text_) : text(text_) {}
        C_Text(FontFamily font_family_) : font_family(font_family_) {}
        C_Text(const String& text_, FontFamily font_family_) : text(text_), font_family(font_family_) {}
        // C_Text(char* text_) : text(text_) {}


        String text;
        FontFamily font_family = FontFamily::BODY;
        std::vector<entt::entity> character_entities;
        entt::entity cursor = entt::null;
        std::vector<int> line_start_character_indicies;
        float scale = 1.0f;
        float max_length;
        float vertical_offset = 0;
        text_align align = text_align::LEFT;
        text_align_vertical align_vertical = text_align_vertical::TOP;
        bool dirty = true;
        bool recalculate_position_flag = true;
        glm::vec4 color = glm::vec4(0, 0, 0, 1);

        int cursor_position = 0;
        bool can_edit = false;
        bool show_cursor = false;
        bool selected = false;

        void change_text(const String& text_) {text = text_; dirty = true; recalculate_position_flag = true;}
        C_Text& set_editable(bool editable = true) {can_edit = editable; return *this;}
        C_Text& set_alignment(text_align a) {align = a; recalculate_position_flag = true; return *this;}
        C_Text& set_vertical_alignment(text_align_vertical av) {align_vertical = av; recalculate_position_flag = true; return *this;}
        C_Text& set_scale(float s) {scale = s; recalculate_position_flag = true; return *this;}
        C_Text& set_pixel_scale(float s) {scale = s / 128.0f; recalculate_position_flag = true; return *this;}
        C_Text& set_color(glm::vec4 c) {color = c; recalculate_position_flag = true; return *this;}
        C_Text& set_alpha(float a) {color.a = a; recalculate_position_flag = true; return *this;}
};

struct C_Character
{
        std::shared_ptr<Character> character;
};