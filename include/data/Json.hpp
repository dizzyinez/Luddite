#pragma once

#include "gason.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <sys/stat.h>

struct Json
{
        // std::string file;
        char* file = nullptr;
        gason::JsonAllocator allocator;
        gason::JsonValue root;
        gason::JsonParseStatus status;
        bool parsed = false;
        Json(std::string file_path)
        {
                struct stat buffer;
                if (stat(file_path.c_str(), &buffer) != 0)
                {
                        return;
                }
                std::ifstream input;
                input.open(file_path);

                input.seekg(0, input.end);
                int length = input.tellg();
                input.seekg(0, input.beg);
                // file.reserve(length);
                file = new char[length];
                input.read(file, length);
                input.close();
                // std::cout.write(file, length);
                // std::cout << "length: " << length << " file: " << file << std::endl;
                status = gason::jsonParse(file, root, allocator);
                if (status != gason::JSON_PARSE_OK)
                {
                        std::cout << "unable to parse file: " << status << std::endl;
                }
                else
                {
                        std::cout << "parsing successful | dir: " << file_path << std::endl;
                        parsed = true;
                }
        }

        ~Json()
        {
                if (file != nullptr)
                        delete[] file;
        }
};
