#ifndef __SWIFTOS__GDT_H
#define __SWIFTOS__GDT_H

#include <common/types.h>

namespace swiftos
{

    class GlobalDescriptorTable
    {
    public:
        class SegmentDescriptor
        {
        private:
            swiftos::common::uint16_t limit_lo;
            swiftos::common::uint16_t base_lo;
            swiftos::common::uint8_t base_hi;
            swiftos::common::uint8_t type;
            swiftos::common::uint8_t flags_limit_hi;
            swiftos::common::uint8_t base_vhi;

        public:
            SegmentDescriptor(swiftos::common::uint32_t base, swiftos::common::uint32_t limit, swiftos::common::uint8_t type);
            swiftos::common::uint32_t Base();
            swiftos::common::uint32_t Limit();

        } __attribute__((packed));

    private:
        // TODO: Not secure.... :/
        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        swiftos::common::uint16_t CodeSegmentSelector();
        swiftos::common::uint16_t DataSegmentSelector();
    };
}

#endif