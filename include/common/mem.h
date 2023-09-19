#ifndef __SWIFTOS__COMMON__MEMSET_H
#define __SWIFTOS__COMMON__MEMSET_H

#include <common/types.h>

namespace swiftos
{
    namespace common
    {
        void *memset(void *dest, int value, common::size_t count)
        {
            if (count == 0)
            {
                return dest;
            }

            common::uint8_t *byte_dest = (common::uint8_t *)dest;
            common::uint32_t dword_value = (common::uint32_t)((value & 0xFF) | (value << 8) | (value << 16) | (value << 24));
            common::size_t dword_count = count / 4;
            common::size_t byte_count = count % 4;

            // Fill memory in dwords
            for (common::size_t i = 0; i < dword_count; ++i)
            {
                *(common::uint32_t *)(byte_dest) = dword_value;
                byte_dest += 4;
            }

            // Fill remaining bytes
            for (common::size_t i = 0; i < byte_count; ++i)
            {
                *(byte_dest++) = (common::uint8_t)value;
            }

            return dest;
        }

        void *memcpy(void *dest, const void *src, common::size_t count)
        {
            common::uint8_t *byte_dest = (common::uint8_t *)dest;
            const common::uint8_t *byte_src = (const common::uint8_t *)src;

            // Check for overlap and copy backwards if needed
            if (byte_src < byte_dest && byte_dest < byte_src + count)
            {
                byte_dest += count;
                byte_src += count;
                for (common::size_t i = 0; i < count; ++i)
                {
                    *(--byte_dest) = *(--byte_src);
                }
            }
            else
            {
                for (common::size_t i = 0; i < count; ++i)
                {
                    *(byte_dest++) = *(byte_src++);
                }
            }

            return dest;
        }

        void *memmove(void *dest, const void *src, common::size_t count)
        {
            common::uint8_t *byte_dest = (common::uint8_t *)dest;
            const common::uint8_t *byte_src = (const common::uint8_t *)src;

            if (byte_dest < byte_src)
            {
                for (common::size_t i = 0; i < count; ++i)
                {
                    *(byte_dest++) = *(byte_src++);
                }
            }
            else if (byte_src < byte_dest)
            {
                byte_dest += count;
                byte_src += count;
                for (common::size_t i = 0; i < count; ++i)
                {
                    *(--byte_dest) = *(--byte_src);
                }
            }

            return dest;
        }

    } // namespace common

} // namespace swiftos

#endif