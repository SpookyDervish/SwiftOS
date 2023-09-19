#ifndef __SWIFTOS__DRIVERS__AMD_AM79C973_H
#define __SWIFTOS__DRIVERS__AMD_AM79C973_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/pci.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>

namespace swiftos
{
    namespace drivers
    {
        class amd_am79c973;

        class RawDataHandler
        {
        protected:
            amd_am79c973 *backend;

        public:
            RawDataHandler(amd_am79c973 *backend);
            ~RawDataHandler();

            virtual bool OnRawDataReceived(common::uint8_t *buffer, common::uint32_t size);
            // OnRawDataReceived must be virtual
            void Send(common::uint8_t *buffer, common::uint32_t size);
        };

        class amd_am79c973 : public Driver, public hardwarecommunication::InterruptHandler
        {
            struct InitializationBlock
            {
                // 16 bits for mode
                common::uint16_t mode;
                // 4 reserved bits
                unsigned reserved1 : 4;
                // number of sent buffers
                unsigned numSendBuffers : 4;
                // another 4 reserved bits
                unsigned reserved2 : 4;
                // number of received buffers
                unsigned numRecvBuffers : 4;
                // mac address or something
                common::uint64_t physicalAddress : 48;
                // ANOTHER 4 RESERVED BITS SKOGJOSFGJKJDSFHGKJDFGHFKG
                unsigned reserved3 : 4;
                // logical address, might be for the ip address not sure at the time of writing this comment. 31 Aug, 2023
                // dont even know why we're concerned about the ip though especially at this level
                common::uint64_t logicalAddress;

                // addresses
                common::uint32_t recvBufferDescrAddress;
                common::uint32_t sendBufferDescrAddress;
            } __attribute__((packed));

            struct BufferDescriptor
            {
                common::uint32_t address;
                common::uint32_t flags;
                /*
                sending a package :
                "Flags2" shows whether an error occurred while sending and should therefore be set to 0 by the driver.
                The OWN bit must now be set in the “flags” field(0x80000000) in order to “transfer” the descriptor to the card.
                Furthermore, STP(Start of Packet, 0x02000000) and ENP(End of Packet, 0x01000000) should be set - this indicates
                that the data is not split up, but that it is a single Ethernet packet.Furthermore, bits 12 - 15 must be set(0x0000F000,
                are probably reserved) and bits 0 - 11 are negative Size of the package.common::uint32_t flags2;
                */
                common::uint32_t flags2;
                common::uint32_t avail;
            } __attribute__((packed));

            hardwarecommunication::Port16Bit MACAddress0Port;
            hardwarecommunication::Port16Bit MACAddress2Port;
            hardwarecommunication::Port16Bit MACAddress4Port;
            hardwarecommunication::Port16Bit registerDataPort;
            hardwarecommunication::Port16Bit registerAddressPort;
            hardwarecommunication::Port16Bit resetPort;
            hardwarecommunication::Port16Bit busControlRegisterDataPort;

            // The main purpose of the initialization block is to hold a pointer to the array
            // of BufferDescriptors, which hold the pointers to the buffers
            InitializationBlock initBlock;

            // 16 byte alligned buffers, 8 send 8 receive
            BufferDescriptor *sendBufferDescr;
            common::uint8_t sendBufferDescrMemory[2048 + 15]; // allocate ram for send buffers
            common::uint8_t sendBuffers[2 * 1024 + 15][8];    // 2 Kib + 15 for allignment
            common::uint8_t currentSendBuffer;                // currently active send buffer

            BufferDescriptor *recvBufferDescr;
            common::uint8_t recvBufferDescrMemory[2048 + 15]; // allocate ram for receive buffers
            common::uint8_t recvBuffers[2 * 1024 + 15][8];
            common::uint8_t currentRecvBuffer; // currently active receive buffer

            RawDataHandler* handler;
        public:
            amd_am79c973(hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor *dev,
                         hardwarecommunication::InterruptManager *interrupts);
            ~amd_am79c973();

            void Activate();
            int Reset();
            common::uint32_t HandleInterrupt(common::uint32_t esp);

            void Send(common::uint8_t *buffer, int count);
            void Receive();

            void SetHandler(RawDataHandler* handler);
            common::uint64_t GetMACAddress();

            // ARP stuff (Address Resolution Protocol)
            void SetIPAddress(common::uint32_t); // We will set IP address manually , but it could also be set using DHCP
            common::uint32_t GetIPAddress();
        };
    } // namespace drivers

} // namespace swiftos

#endif