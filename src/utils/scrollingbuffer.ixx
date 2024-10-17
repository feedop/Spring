export module scrollingbuffer;

import std;

import <imgui.h>;

export
template<int size>
struct ScrollingBuffer
{
     int offset;
     ImVector<ImVec2> data;
     ScrollingBuffer()
     {
         offset = 0;
         data.reserve(size);
     }
     void addPoint(float x, float y)
     {
         if (data.size() < size)
             data.push_back(ImVec2(x, y));
         else
         {
             data[offset] = ImVec2(x, y);
             offset = (offset + 1) % size;
         }
     }
     void erase()
     {
         if (data.size() > 0)
         {
             data.shrink(0);
             offset = 0;
         }
     }
};