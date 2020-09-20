#include "data/Resources.hpp"

std::shared_ptr<Json> JsonManager::Allocate(std::string file_path)
{
        return std::make_shared<Json>(file_path);
}
