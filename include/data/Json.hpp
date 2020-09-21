#pragma once

#include "gason.hpp"
#include <fstream>
#include <string>
#include <iostream>

struct Json
{
        char* file = nullptr;
        gason::JsonAllocator allocator;
        gason::JsonValue root;
        gason::JsonParseStatus status;
        Json(std::string file_path)
        {
                std::ifstream input;
                input.open(file_path);

                input.seekg(0, input.end);
                int length = input.tellg();
                input.seekg(0, input.beg);
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
                        // std::cout << "parsing successful" << std::endl;
                }
        }
        ~Json()
        {
                delete file;
        }
};
