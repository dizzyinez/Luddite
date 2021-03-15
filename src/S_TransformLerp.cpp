#include "systems/TransformLerp.hpp"
#include "entt.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "components/GameComponents.hpp"
#include <glm/gtx/compatibility.hpp>//lerp
#include <glm/gtx/string_cast.hpp>



void S_Transform_Lerp::update(entt::registry& before_reg, entt::registry& after_reg, entt::registry& lerp_reg, float alpha)
{
        lerp_reg.view<C_Position>().each([&before_reg, &after_reg, alpha](auto Entity, C_Position &pos)
        {
                if (before_reg.valid(Entity) && before_reg.has<C_Position>(Entity))
                {
                        const auto &last_pos = before_reg.get<C_Position>(Entity).position;
                        const auto &curr_pos = after_reg.get<C_Position>(Entity).position;
                        pos.position = glm::lerp(last_pos, curr_pos, alpha);
                        std::cout << glm::to_string(pos.position) << std::endl; 
                }
        });
        lerp_reg.view<C_Size>().each([&before_reg, &after_reg, alpha](auto Entity, C_Size &size)
        {
                if (before_reg.valid(Entity) && before_reg.has<C_Size>(Entity))
                {
                        const auto &last_size = before_reg.get<C_Size>(Entity).size;
                        const auto &curr_size = after_reg.get<C_Size>(Entity).size;
                        size.size = glm::lerp(last_size, curr_size, alpha);
                }
        });
}