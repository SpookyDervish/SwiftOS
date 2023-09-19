#ifndef __SWIFTOS__SYSCALLS_H
#define __SWIFTOS__SYSCALLS_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <multitasking.h>

namespace swiftos
{

    class SyscallHandler : public hardwarecommunication::InterruptHandler
    {

    public:
        SyscallHandler(hardwarecommunication::InterruptManager* interruptManager, swiftos::common::uint8_t InterruptNumber);
        ~SyscallHandler();

        virtual swiftos::common::uint32_t HandleInterrupt(swiftos::common::uint32_t esp);

    };


}


#endif