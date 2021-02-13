#pragma once
#include <vector>
#include <string>

#define CHARACTER_TYPES_DECLARE \
        CHARACTER_TYPE_DECLARATION(character) \
        CHARACTER_TYPE_DECLARATION(ping_pong) \

struct Characters
{
        enum class eCharacter
        {
        #define CHARACTER_TYPE_DECLARATION(name) name,
                CHARACTER_TYPES_DECLARE
        #undef CHARACTER_TYPE_DECLARATION
        };


        inline static const std::string Character_Ids[] =
        {
        #define CHARACTER_TYPE_DECLARATION(name) #name,
                CHARACTER_TYPES_DECLARE
        #undef CHARACTER_TYPE_DECLARATION
        };

        static std::string GetTextureFilePath(eCharacter c)
        {
                auto id = static_cast<uint32_t>(c);
                std::string buffer = "../assets/characters/";
                buffer += Character_Ids[id];
                buffer += "/";
                buffer += Character_Ids[id];
                buffer += ".png";
                return buffer;
                // return "../assets/characters/" + Character_Ids[id] + "/" + Character_Ids[id] + ".png";
        };

        static std::string GetAnimationFilePath(eCharacter c)
        {
                auto id = static_cast<uint32_t>(c);
                std::string buffer = "../assets/characters/";
                buffer += Character_Ids[id];
                buffer += "/";
                buffer += Character_Ids[id];
                buffer += ".animation";
                return buffer;
                // return "../assets/characters/" + Character_Ids[id] + "/" + Character_Ids[id] + ".png";
        };
        static std::string GetAnimationBehaviorFilePath(eCharacter c)
        {
                auto id = static_cast<uint32_t>(c);
                std::string buffer = "../assets/characters/";
                buffer += Character_Ids[id];
                buffer += "/";
                buffer += Character_Ids[id];
                buffer += ".lua";
                return buffer;
                // return "../assets/characters/" + Character_Ids[id] + "/" + Character_Ids[id] + ".png";
        };
};








using AssetID = std::uint16_t;
namespace Assets
{
namespace Sprite
{
constexpr const AssetID player = 0;
constexpr const AssetID untitled = 1;
constexpr const int count = 2;

//couldn't find a fucking way to constexpr vectors and whatnot (even though that's supposed to be in c++20) so this is the retardation we get
struct data
{
        data(std::string file_path_, std::vector<uint8_t> animation_frames_) //we won't need more than 8 seconds for an animation (256frames/30fps)
                : file_path(file_path_), animation_frames(animation_frames_)
        {
        }
        std::string file_path;
        std::vector<uint8_t> animation_frames;
};
const data Data[] = {
        data("../assets/textures/anim.png", std::vector<uint8_t>(50)),
        data("../assets/textures/untitled.png", std::vector<uint8_t>(50))
};

inline uint32_t texture_slots[count];
uint32_t Get(AssetID id);

//
// constexpr char* file_names[] =
// {
//         "../assets/textures/player.png",
//         "../assets/textures/untitled.png"
// };
}
}
