#pragma once
#include <vector>
#include <array>
#include <string>
#include "utils/StringFormat.hpp"

#define CHARACTER_TYPES_DECLARE \
        CHARACTER_TYPE_DECLARATION(character) \
        CHARACTER_TYPE_DECLARATION(ping_pong) \

struct Characters
{
        enum class eCharacter : uint32_t
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

        #define GET_FILE_PATH_FUNCTION(FUNCTION_NAME, FILE_TYPE) \
        static std::string FUNCTION_NAME(eCharacter c) \
        { \
                auto id = static_cast<uint32_t>(c); \
                return string_format("../assets/characters/%s/%s.%s", Character_Ids[id].c_str(), Character_Ids[id].c_str(), #FILE_TYPE); \
        };
        GET_FILE_PATH_FUNCTION(GetTextureFilePath, png)
        GET_FILE_PATH_FUNCTION(GetAnimationFilePath, animation)
        GET_FILE_PATH_FUNCTION(GetAnimationBehaviorFilePath, lua)
        #undef GET_FILE_PATH_FUNCTION
};


#define NPC_TYPES_DECLARE \
        NPC_TYPE_DECLARATION(lizard) \

struct Npcs
{
        enum class eNpc : uint32_t
        {
        #define NPC_TYPE_DECLARATION(name) name,
                NPC_TYPES_DECLARE
        #undef NPC_TYPE_DECLARATION
        };

        inline static const std::string Npc_Ids[] =
        {
        #define NPC_TYPE_DECLARATION(name) #name,
                NPC_TYPES_DECLARE
        #undef NPC_TYPE_DECLARATION
        };

        #define GET_FILE_PATH_FUNCTION(FUNCTION_NAME, FILE_TYPE) \
        static std::string FUNCTION_NAME(eNpc c) \
        { \
                auto id = static_cast<uint32_t>(c); \
                return string_format("../assets/npc/%s/%s.%s", Npc_Ids[id].c_str(), Npc_Ids[id].c_str(), #FILE_TYPE); \
        };
        GET_FILE_PATH_FUNCTION(GetTextureFilePath, png)
        GET_FILE_PATH_FUNCTION(GetAnimationFilePath, animation)
        GET_FILE_PATH_FUNCTION(GetAnimationBehaviorFilePath, lua)
        #undef GET_FILE_PATH_FUNCTION
};


#define SOUNDS_DECLARE \
        SOUND_DECLARATION(whoosh1) \
        SOUND_DECLARATION(whoosh2) \

struct Sounds
{
        enum class eSound : uint32_t
        {
        #define SOUND_DECLARATION(name) name,
                SOUNDS_DECLARE
        #undef SOUND_DECLARATION
        };

        inline static const std::vector<std::string> Sound_Ids = //std::string Sound_Ids[] =
        {
        #define SOUND_DECLARATION(name) #name,
                SOUNDS_DECLARE
        #undef SOUND_DECLARATION
        };

        inline static const std::vector<std::string> Sound_File_Locations =
        {
        #define SOUND_DECLARATION(name) "../assets/sounds/" #name ".wav",
                SOUNDS_DECLARE
        #undef SOUND_DECLARATION
        };
        static std::string GetFilePath(eSound s)
        {
                auto id = static_cast<uint32_t>(s);
                return Sound_File_Locations[id];
        }
};






// using AssetID = std::uint16_t;
// namespace Assets
// {
// namespace Sprite
// {
// constexpr const AssetID player = 0;
// constexpr const AssetID untitled = 1;
// constexpr const int count = 2;

// //couldn't find a fucking way to constexpr vectors and whatnot (even though that's supposed to be in c++20) so this is the retardation we get
// struct data
// {
//         data(std::string file_path_, std::vector<uint8_t> animation_frames_) //we won't need more than 8 seconds for an animation (256frames/30fps)
//                 : file_path(file_path_), animation_frames(animation_frames_)
//         {
//         }
//         std::string file_path;
//         std::vector<uint8_t> animation_frames;
// };
// const data Data[] = {
//         data("../assets/textures/anim.png", std::vector<uint8_t>(50)),
//         data("../assets/textures/untitled.png", std::vector<uint8_t>(50))
// };

// inline uint32_t texture_slots[count];
// uint32_t Get(AssetID id);

// //
// // constexpr char* file_names[] =
// // {
// //         "../assets/textures/player.png",
// //         "../assets/textures/untitled.png"
// // };
// }
// }
