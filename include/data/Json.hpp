#pragma once

#include "gason.hpp"
#include <fstream>
#include <iostream>

struct Json
{
  gason::JsonAllocator allocator;
  gason::JsonValue root;
  gason::JsonParseStatus status;
  Json(std::string file_path)
  {
    istream input(file_path);
    if (input.is_open())
    {
      std::cout << "open" << std::endl;
    }
  }
  ~Json()
  {

  }
};
