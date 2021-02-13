#pragma once
#include <string>
#include <locale>
#include <codecvt>

// typedef std::basic_string<char32_t> String;
struct String : public std::basic_string<char32_t>
{
        using std::basic_string<char32_t>::basic_string;
        // String() = default;
        // String(const String& str_) {*this = str_;}
        // String(std::basic_string<char32_t> str_) {*this = str_;}
        // String(const char32_t* char_) {*this = std::basic_string<char32_t>(char_);}
        operator const std::string()
        {
                std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
                auto p = reinterpret_cast<const char32_t *>(this->data());
                return convert.to_bytes(p, p + this->size());
        }
};