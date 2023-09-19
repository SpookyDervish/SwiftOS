#ifndef __SWIFTOS__DRIVERS__KEYBOARD_H
#define __SWIFTOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>

namespace swiftos
{
    namespace drivers
    {

        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();
            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };

        class KeyboardDriver : public swiftos::hardwarecommunication::InterruptHandler, public Driver
        {
            swiftos::hardwarecommunication::Port8Bit dataport;
            swiftos::hardwarecommunication::Port8Bit commandport;

            KeyboardEventHandler *handler;

        public:
            KeyboardDriver(swiftos::hardwarecommunication::InterruptManager *manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual swiftos::common::uint32_t HandleInterrupt(swiftos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}

#endif