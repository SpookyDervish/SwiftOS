#ifndef __SWIFTOS__HARDWARECOMMUNICATION__PCI_H
#define __SWIFTOS__HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecommunication/interrupts.h>

#include <memorymanagement.h>

namespace swiftos
{
    namespace hardwarecommunication
    {

        enum BaseAddressRegisterType
        {
            MemoryMapping = 0,
            InputOutput = 1
        };
        
        
        
        class BaseAddressRegister
        {
        public:
            bool prefetchable;
            swiftos::common::uint8_t* address;
            swiftos::common::uint32_t size;
            BaseAddressRegisterType type;
        };
        
        
        
        class PeripheralComponentInterconnectDeviceDescriptor
        {
        public:
            swiftos::common::uint32_t portBase;
            swiftos::common::uint32_t interrupt;
            
            swiftos::common::uint16_t bus;
            swiftos::common::uint16_t device;
            swiftos::common::uint16_t function;

            swiftos::common::uint16_t vendor_id;
            swiftos::common::uint16_t device_id;
            
            swiftos::common::uint8_t class_id;
            swiftos::common::uint8_t subclass_id;
            swiftos::common::uint8_t interface_id;

            swiftos::common::uint8_t revision;
            
            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();
            
        };


        class PeripheralComponentInterconnectController
        {
            Port32Bit dataPort;
            Port32Bit commandPort;
            
        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();
            
            swiftos::common::uint32_t Read(swiftos::common::uint16_t bus, swiftos::common::uint16_t device, swiftos::common::uint16_t function, swiftos::common::uint32_t registeroffset);
            void Write(swiftos::common::uint16_t bus, swiftos::common::uint16_t device, swiftos::common::uint16_t function, swiftos::common::uint32_t registeroffset, swiftos::common::uint32_t value);
            bool DeviceHasFunctions(swiftos::common::uint16_t bus, swiftos::common::uint16_t device);
            
            void SelectDrivers(swiftos::drivers::DriverManager* driverManager, swiftos::hardwarecommunication::InterruptManager* interrupts);
            swiftos::drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, swiftos::hardwarecommunication::InterruptManager* interrupts);
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(swiftos::common::uint16_t bus, swiftos::common::uint16_t device, swiftos::common::uint16_t function);
            BaseAddressRegister GetBaseAddressRegister(swiftos::common::uint16_t bus, swiftos::common::uint16_t device, swiftos::common::uint16_t function, swiftos::common::uint16_t bar);
        };

    }
}
    
#endif