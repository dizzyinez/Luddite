#pragma once
#include "data/ResourceManager.hpp"
#include "data/Texture.hpp"
#include "data/Json.hpp"

class TextureManager : public ResourceManager<Texture>
{
        std::shared_ptr<Texture> Allocate(std::string file_path);
};

class JsonManager : public ResourceManager<Json>
{
        std::shared_ptr<Json> Allocate(std::string file_path);
};

static TextureManager textures;
static JsonManager json_manager;
