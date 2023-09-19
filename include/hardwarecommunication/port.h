#ifndef __SWIFTOS__HARDWARECOMMUNICATION__PORT_H
#define __SWIFTOS__HARDWARECOMMUNICATION__PORT_H

#include <common/types.h>

namespace swiftos
{
    namespace hardwarecommunication
    {

        class Port
        {
            protected:
                Port(swiftos::common::uint16_t portnumber);
                // FIXME: Must be virtual (currently isnt because the kernel has no memory management yet)
                ~Port();
                swiftos::common::uint16_t portnumber;
        };

        
        class Port8Bit : public Port
        {
            public:
                Port8Bit(swiftos::common::uint16_t portnumber);
                ~Port8Bit();

                virtual swiftos::common::uint8_t Read();
                virtual void Write(swiftos::common::uint8_t data);

            protected:
                static inline swiftos::common::uint8_t Read8(swiftos::common::uint16_t _port)
                {
                    swiftos::common::uint8_t result;
                    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));
                    return result;
                }

                static inline void Write8(swiftos::common::uint16_t _port, swiftos::common::uint8_t _data)
                {
                    __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
                }
        };



        class Port8BitSlow : public Port8Bit
        {
            public:
                Port8BitSlow(swiftos::common::uint16_t portnumber);
                ~Port8BitSlow();

                virtual void Write(swiftos::common::uint8_t data);
            protected:
                static inline void Write8Slow(swiftos::common::uint16_t _port, swiftos::common::uint8_t _data)
                {
                    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
                }

        };



        class Port16Bit : public Port
        {
            public:
                Port16Bit(swiftos::common::uint16_t portnumber);
                ~Port16Bit();

                virtual swiftos::common::uint16_t Read();
                virtual void Write(swiftos::common::uint16_t data);

            protected:
                static inline swiftos::common::uint16_t Read16(swiftos::common::uint16_t _port)
                {
                    swiftos::common::uint16_t result;
                    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port));
                    return result;
                }

                static inline void Write16(swiftos::common::uint16_t _port, swiftos::common::uint16_t _data)
                {
                    __asm__ volatile("outw %0, %1" : : "a" (_data), "Nd" (_port));
                }
        };



        class Port32Bit : public Port
        {
            public:
                Port32Bit(swiftos::common::uint16_t portnumber);
                ~Port32Bit();

                virtual swiftos::common::uint32_t Read();
                virtual void Write(swiftos::common::uint32_t data);

            protected:
                static inline swiftos::common::uint32_t Read32(swiftos::common::uint16_t _port)
                {
                    swiftos::common::uint32_t result;
                    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port));
                    return result;
                }

                static inline void Write32(swiftos::common::uint16_t _port, swiftos::common::uint32_t _data)
                {
                    __asm__ volatile("outl %0, %1" : : "a"(_data), "Nd" (_port));
                }
        };

    }
}


#endif