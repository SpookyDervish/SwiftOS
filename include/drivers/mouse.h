#ifndef __SWIFTOS__DRIVERS__MOUSE_H
#define __SWIFTOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>

namespace swiftos
{
    namespace drivers
    {

        class MouseEventHandler
        {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(swiftos::common::uint8_t button);
            virtual void OnMouseUp(swiftos::common::uint8_t button);
            virtual void OnMouseMove(int x, int y);
        };

        class MouseDriver : public swiftos::hardwarecommunication::InterruptHandler, public Driver
        {
            swiftos::hardwarecommunication::Port8Bit dataport;
            swiftos::hardwarecommunication::Port8Bit commandport;
            swiftos::common::uint8_t buffer[3];
            swiftos::common::uint8_t offset;
            swiftos::common::uint8_t buttons;

            MouseEventHandler *handler;

        public:
            MouseDriver(swiftos::hardwarecommunication::InterruptManager *manager, MouseEventHandler *handler);
            ~MouseDriver();
            virtual swiftos::common::uint32_t HandleInterrupt(swiftos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}

#endif