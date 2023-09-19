#ifndef __SWIFTOS__COMMON__MATH_H
#define __SWIFTOS__COMMON__MATH_H

#include <common/types.h>

namespace swiftos
{
    namespace common
    {
        uint32_t min(uint32_t a, uint32_t b)
        {
            return (a < b) ? a : b;
        }

        uint32_t max(uint32_t a, uint32_t b)
        {
            return (a > b) ? a : b;
        }

        unsigned long int rand_next = 1;

        unsigned int rand()
        {
            rand_next = rand_next * 1103515245 + 12345;
            return ((unsigned int)(rand_next / 65536) % 32768);
        }

        int floor(int num)
        {
            int x = (int)num;
            return num < x ? x - 1 : x;
        }

        int abs(int num) { return num < 0 ? -num : num; }
    } // namespace common

} // namespace swiftos

#endif