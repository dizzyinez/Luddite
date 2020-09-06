#ifndef C_Tileset_hpp
#define C_Tileset_hpp

struct C_Tileset
{
        C_Tileset( uint16_t tiles_width_, uint16_t tiles_height_, uint32_t index_)
                : tiles_width(tiles_width_), tiles_height(tiles_height_), index(index_)
        {
        }
        uint16_t tiles_width;
        uint16_t tiles_height;
        uint32_t index;
};

struct C_Animation
{
        C_Animation(int16_t line_, int16_t frames_, int16_t current_frame_, double seconds_per_frame_, double timer_, bool repeat_, bool animating_)
                : line (line_), frames (frames_), current_frame (current_frame_), seconds_per_frame (seconds_per_frame_), timer (timer_), repeat (repeat_), animating (animating_)
        {
        }
        int16_t line;
        int16_t frames;
        int16_t current_frame;
        double seconds_per_frame;
        double timer;
        bool repeat = true;
        bool animating = false;
};

#endif
