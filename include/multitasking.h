#ifndef __SWIFTOS__MULTITASKING_H
#define __SWIFTOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace swiftos
{
    struct CPUState
    {
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
        common::uint32_t error;

        // The below registers are pushed automatically by the CPU when an interrupt occurs.
        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;
    } __attribute__((packed));

    class Task
    {
        friend class TaskManager;

    private:
        common::uint8_t stack[4096]; // 4 KiB stack
        CPUState *cpustate;

    public:
        Task(GlobalDescriptorTable *gdt, void entrypoint());
        ~Task();
    };

    class TaskManager
    {
    private:
        Task *tasks[256];
        int numTasks;
        int currentTask;

    public:
        TaskManager();
        ~TaskManager();
        bool AddTask(Task *task);
        CPUState *Schedule(CPUState *cpustate);
    };
} // namespace swiftos

#endif