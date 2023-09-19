#ifndef __SWIFTOS__DRIVERS__DRIVER_H
#define __SWIFTOS__DRIVERS__DRIVER_H

namespace swiftos
{
    namespace drivers
    {

        class Driver
        {
        public:
            Driver();
            ~Driver();

            virtual void Activate();
            virtual int Reset();
            virtual void Deactivate();
        };

        class DriverManager
        {
        public:
            Driver *drivers[0xFF];
            int numDrivers;

        public:
            DriverManager();
            void AddDriver(Driver *drv);

            void ActivateAll();
        };
    }
}

#endif