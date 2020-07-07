#ifndef Input_hpp
#define Input_hpp

enum class Action {NONE, MOUSE, MOTION, FIRE1, FIRE2};
enum class Press {NONE, PRESSED, RELEASED};

struct Input
{
        Action action = Action::NONE;
        Press press = Press::NONE;
        glm::vec2 vector = glm::vec2(0.0f,0.0f);
        Input(Action a, Press p, glm::vec2 v) : action(a), press(p), vector(v)
        {
        }

        Input(Action a, Press p, float x, float y) : action(a), press(p)
        {
                vector = glm::vec2(x,y);
        }

        Input(Action a, Press p) : action(a), press(p)
        {
                vector = glm::vec2(0.0f,0.0f);
        }

        Input(Action a) : action(a)
        {
                press = Press::NONE;
                vector = glm::vec2(0.0f,0.0f);
        }
};

#endif
