#ifndef Input_hpp
#define Input_hpp

#include "events/Events.hpp"

/* ----------  Keyboard  ----------  */

struct E_Keyboard : public Event<E_Keyboard>
{
        E_Keyboard(int key_, int scancode_, int action_, int mods_) : key(key_), scancode(scancode_), action(action_), mods(mods_)
        {
        }
        uint16_t key;
        uint16_t scancode;
        uint16_t action;
        uint16_t mods;
};

struct E_KeyPress : public Event<E_KeyPress>
{
        E_KeyPress(int key_, int scancode_, int mods_) : key(key_), scancode(scancode_), mods(mods_)
        {
        }
        int key;
        int scancode;
        int mods;
};

struct E_KeyRelease : public Event<E_KeyRelease>
{
        E_KeyRelease(int key_, int scancode_, int mods_) : key(key_), scancode(scancode_), mods(mods_)
        {
        }
        int key;
        int scancode;
        int mods;
};

struct E_KeyRepeat : public Event<E_KeyRepeat>
{
        E_KeyRepeat(int key_, int scancode_, int mods_) : key(key_), scancode(scancode_), mods(mods_)
        {
        }
        int key;
        int scancode;
        int mods;
};

struct E_KeyPressAndRepeat : public Event<E_KeyPressAndRepeat>
{
        E_KeyPressAndRepeat(int key_, int scancode_, int mods_) : key(key_), scancode(scancode_), mods(mods_)
        {
        }
        int key;
        int scancode;
        int mods;
};



/* ----------  Mouse  ----------  */
struct E_CusrsorMotion : public Event<E_CusrsorMotion>
{
        E_CusrsorMotion(double &xpos_, double &ypos_) : xpos(xpos_), ypos(ypos_)
        {
        }
        double xpos;
        double ypos;
};

struct E_MouseButton : public Event<E_MouseButton>
{
        E_MouseButton(int button_, int action_, int mods_, double &xpos_, double &ypos_)
                : button(button_), action(action_), mods(mods_), xpos(xpos_), ypos(ypos_)
        {
        }
        int button;
        int action;
        int mods;
        double xpos;
        double ypos;
};

struct E_MouseButtonPress : public Event<E_MouseButtonPress>
{
        E_MouseButtonPress(int button_, int mods_) : button(button_), mods(mods_)
        {
        }
        int button;
        int mods;
};

struct E_MouseButtonRelease : public Event<E_MouseButtonRelease>
{
        E_MouseButtonRelease(int button_, int mods_) : button(button_), mods(mods_)
        {
        }
        int button;
        int mods;
};




/* ----------  Window  ----------  */
struct E_WindowResize : public Event<E_WindowResize>
{
        E_WindowResize(int width_, int height_) : width(width_), height(height_)
        {
        }
        int width;
        int height;
};



#endif
