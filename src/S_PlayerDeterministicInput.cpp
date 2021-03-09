#include "systems/PlayerDeterministicInput.hpp"
#include "components/GameComponents.hpp"

void S_PlayerDeterministicInput::update(float deltaTime, entt::registry &reg)
{
        reg.view<C_PlayerInput, C_PlayerDeterministicInput>().each([&reg](auto Entity, auto &input, auto& d_input) {
                // auto& d_input = reg.get<C_PlayerDeterministicInput>(Entity);
        #define TEST_BUTTON_PRESSES_AND_RELEASES(button, index) \
        d_input.button ## _press_age++; \
        d_input.button ## _release_age++; \
        if (d_input.buttons_previous[index] == 0 && input.buttons[index] == 1) \
        d_input.button ## _press_age = 0; \
        if (d_input.buttons_previous[index] == 1 && input.buttons[index] == 0) \
        d_input.button ## _release_age = 0;
                TEST_BUTTON_PRESSES_AND_RELEASES(button1, 4)
                TEST_BUTTON_PRESSES_AND_RELEASES(button2, 5)
                TEST_BUTTON_PRESSES_AND_RELEASES(button3, 6)
                TEST_BUTTON_PRESSES_AND_RELEASES(button4, 7)
                d_input.buttons_previous = input.buttons;
        });
}