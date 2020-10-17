#pragma once
#include "entt.hpp"

struct C_Simulation
{
        uint32_t object_id = 0;
};

struct C_StoredFrames
{
        static constexpr uint8_t max_frames = 20;
        std::array<entt::registry, max_frames> frame_array;
        uint64_t start_frame = 0;
        int index = 0;
        entt::registry& last_frame()
        {
                if (index > 0)
                {
                        return frame_array[index - 1];
                }
                else
                {
                        return frame_array[max_frames - 1];
                }
        }
        void increment_frame()
        {
                index++;
                if (index >= max_frames)
                        index = 0;
        }
};
