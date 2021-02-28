#include "data/SoundAllocator.hpp"

#include <memory>
#include <vector>

template <>
std::shared_ptr<Sound> ResourceAllocator<Sound>::Allocate(std::string file_path)
{
        std::shared_ptr<Sound> sound = std::make_shared<Sound>();
        sound->wav.load(file_path.c_str());
        return sound;
}
