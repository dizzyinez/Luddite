#pragma once
#include "layers/Layer.hpp"
#include "devtools/CharacterRenderer.hpp"

class L_CharacterEditor : public Layer
{
public:
        void init();
        void handleEvents(float deltaTime);
        void update(float deltaTime);
        void render(float alpha, float deltaTime);
        void clean();
        L_CharacterEditor();
private:
        CR::CharacterRenderer character_renderer;
};