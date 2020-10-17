#ifndef game_hpp
#define game_hpp

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Layer;

class Game
{
public:
        bool running;

        Game();
        ~Game();
        bool Init(GLFWwindow* w);
        void Update(float deltaTime);
        void Render(float alpha);
        void Clean();

        void PushLayer(Layer* Layer);
        void PopLayer(Layer* layer);

        GLFWwindow* window;
private:
        std::vector<Layer*> Layers;
};

#endif
