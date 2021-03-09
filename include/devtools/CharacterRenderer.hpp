#pragma once

#include "OBJ_Loader.h"
#include <string>
#include "data/Texture.hpp"
#include "rendering/Shader.hpp"
#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>

#include "core/defines.h"
namespace CR
{
struct Hitbox
{
        std::array<glm::vec3, 8> positions;
};

struct Tile
{
        glm::ivec2 pos;
        glm::ivec2 size;
        glm::ivec2 origin;
};

struct Frame
{
        Frame() = default;
        std::vector<Hitbox> hitboxes;
        std::array<Tile, TOTAL_DIRECTIONS> from_tiles;
        std::array<Tile, TOTAL_DIRECTIONS> tiles;
};

struct Animation
{
        Animation(const std::string& name_)
                : name(name_) {}
        std::string name;
        unsigned int fps;
        // unsigned int total_frames;
        std::vector<Frame> frames;
        unsigned char* animation_image_buffer;
        int image_width;
        int image_height;
};

struct Character
{
        Character(const std::string& name_)
                : name(name_) {}
        std::string name;
        std::unordered_map<std::string, Animation> animations;
        Animation& GetAnimation(const std::string& animation);
        const int frame_size = 128;
        void Export();
        //map of textures for each animation
        // std::map<std::string, std::shared_ptr<Texture> >;
};

class CharacterRenderer
{
public:
        CharacterRenderer();
        std::shared_ptr<Texture> RenderAnimation(const std::string& character_name, const std::string& animation_name);
        void ExportCharacter(const std::string& character_name);
        void LoadCharacter(const std::string& character_name);

private:
        // objl::Loader loader;
        // inline static Shader shader;
        // glm::mat4 mvp;
        std::unordered_map<std::string, Character> characters;
        Character& GetCharacter(const std::string& character_name);


        struct Vertex
        {
                glm::vec3 position;
                glm::vec3 normal;
                float color;
        };
};
}