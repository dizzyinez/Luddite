#pragma once
#include "entt.hpp"

struct C_Simulation
{
        uint32_t object_id = 0;
};

struct C_StoredFrames
{
        static constexpr uint8_t max_frames = 120;
        std::array<entt::registry, max_frames> frame_array;
        uint64_t start_frame_id = 0;
        int index = 0;
        uint64_t current_frame_id() const
        {
                return start_frame_id + index;
        }
        int frame_id_to_index(uint64_t frame) const
        {
                return frame - start_frame_id;
        }
        // entt::registry& last_frame()
        // {
        //         if (index > 0)
        //         {
        //                 return frame_array[index - 1];
        //         }
        //         else
        //         {
        //                 return frame_array[max_frames - 1];
        //         }
        // }
        void increment_index()
        {
                index++;
                if (index >= max_frames)
                {
                        index = max_frames - 1;
                        std::cout << "ERROR: Maximum stored frames reached!" << std::endl;
                }
        }
};
