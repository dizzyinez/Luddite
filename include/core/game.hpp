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
        inline static bool running;

        // Game();
        // ~Game();
        static bool Init(GLFWwindow* w);
        static void Update(float deltaTime);
        static void Render(float alpha);
        static void Clean();

        static void PushLayer(Layer*layer);
        static void PopLayer(Layer* layer);
        static inline int window_width;
        static inline int window_height;

        inline static GLFWwindow* window;
private:
        static void push_layer(Layer* layer);
        static void pop_layer(Layer* layer);
        static void update_queue();

        inline static std::vector<Layer*> Layers;
        inline static std::vector<Layer*> add_queue;
        inline static std::vector<Layer*> remove_queue;
};

#endif
