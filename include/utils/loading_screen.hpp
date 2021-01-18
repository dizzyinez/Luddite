#include <string>
#include "systems/System.hpp"

namespace utils
{
struct load_screen
{
        static void initialize();
        static void display(const std::string& text_);
        static void clean();
        inline static SystemManager systems;
        inline static entt::registry registry;
        inline static entt::entity text;
};
}