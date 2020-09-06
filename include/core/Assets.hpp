#ifndef Assets_hpp
#define Assets_hpp

#include <vector>
#include <string>

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
        data("../assets/textures/anim.png", std::vector<uint8_t>(22)),
        data("../assets/textures/untitled.png", std::vector<uint8_t>(22))
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
#endif
