#include "utils/loading_screen.hpp"
#include "systems/Gui.hpp"
#include "systems/Draw.hpp"
#include "systems/TextRendering.hpp"
#include "core/game.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "components/Position.hpp"
#include "components/Size.hpp"
#include "components/Gui.hpp"
#include "components/text.hpp"
#include "components/Texture.hpp"
#include "components/DrawLayer.hpp"
// #include "components
// #include "components
// #include "components

namespace utils
{
void load_screen::initialize()
{
        systems.add<S_Gui_Input,
                    S_Gui,
                    S_Text_Rendering,
                    S_Draw>();
        systems.configure(registry, nullptr);
        text = registry.create();
        registry.emplace<C_Position>(text);
        registry.emplace<C_Size>(text);
        registry.emplace<C_DrawLayer>(text, DrawLayer::gui);
        registry.emplace<C_Tint>(text, glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
        registry.emplace<C_Gui>(text)
        .set_text_scale(0.9f, dimensions::HEIGHT)
        .set_x(CenterConstraint(dimensions::WIDTH))
        .set_y(CenterConstraint(dimensions::HEIGHT))
        .set_w(PercentConstriant(0.7f, dimensions::WIDTH))
        .set_h(AspectConstraint(8.0f));
        registry.emplace<C_Text>(text);
}
void load_screen::display(const std::string& text_)
{
        // registry.get<C_Text>(text).change_text(text_);

        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        // systems.update<S_Gui_Input>(0.0f, registry);
        // systems.update<S_Gui>(0.0f, registry);
        // systems.update<S_Text_Rendering>(0.0f, registry);
        // systems.update<S_Draw>(0.0f, registry);
        glfwSwapBuffers(Game::window);
}
void load_screen::clean()
{
}
}