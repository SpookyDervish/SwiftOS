#ifndef __SWIFTOS__COMMON__STRING_H
#define __SWIFTOS__COMMON__STRING_H

#include <common/types.h>

namespace swiftos
{
    namespace common
    {
        common::size_t strlen(const char *str)
        {
            common::size_t len = 0;
            while (str[len] != '\0')
            {
                len++;
            }
            return len;
        }

        void strclat(char *dest, const char *src, common::size_t dest_size)
        {
            common::size_t dest_len = strlen(dest);
            common::size_t src_len = strlen(src);

            common::size_t i, j;
            for (i = dest_len, j = 0; i < dest_size - 1 && j < src_len; i++, j++)
            {
                dest[i] = src[j];
            }
            dest[i] = '\0';
        }

        size_t strlcpy(char *dest, const char *src, size_t dest_size)
        {
            size_t src_len = strlen(src);
            size_t copy_len = src_len < dest_size ? src_len : dest_size - 1;

            size_t i;
            for (i = 0; i < copy_len; i++)
            {
                dest[i] = src[i];
            }
            dest[i] = '\0';

            return src_len;
        }
    } // namespace common

} // namespace swiftos

#endif