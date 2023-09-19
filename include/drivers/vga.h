#ifndef __SWIFTOS__DRIVERS__VGA_H
#define __SWIFTOS__DRIVERS__VGA_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace swiftos
{
    namespace drivers
    {

        class VideoGraphicsArray
        {
        protected:
            hardwarecommunication::Port8Bit miscPort;
            hardwarecommunication::Port8Bit crtcIndexPort;
            hardwarecommunication::Port8Bit crtcDataPort;
            hardwarecommunication::Port8Bit sequencerIndexPort;
            hardwarecommunication::Port8Bit sequencerDataPort;
            hardwarecommunication::Port8Bit graphicsControllerIndexPort;
            hardwarecommunication::Port8Bit graphicsControllerDataPort;
            hardwarecommunication::Port8Bit attributeControllerIndexPort;
            hardwarecommunication::Port8Bit attributeControllerReadPort;
            hardwarecommunication::Port8Bit attributeControllerWritePort;
            hardwarecommunication::Port8Bit attributeControllerResetPort;

            void WriteRegisters(common::uint8_t* registers);
            common::uint8_t* GetFrameBufferSegment();

            common::uint8_t *frameBuffer1;
            common::uint8_t *frameBuffer2;
            common::uint8_t *currentFrameBuffer;

            virtual common::uint8_t GetColorIndex(common::uint8_t r, common::uint8_t g, common::uint8_t b);

            common::uint8_t *BUFFER = (common::uint8_t *) 0xA0000;

            // double buffers
            // Stinky bloody downcast, stupid hack (im like those goofy tf2 devs with their silly hacks)
            common::uint8_t _sbuffers[2][320*200];
            common::uint8_t _sback = 0;

            #define CURRENT (_sbuffers[_sback])
            #define SWAP() (_sback = 1 - _sback)

        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            virtual bool SupportsMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);
            virtual bool SetMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);
            virtual void PutPixel(common::int32_t x, common::int32_t y,  common::uint8_t r, common::uint8_t g, common::uint8_t b);
            virtual void PutPixel(common::int32_t x, common::int32_t y, common::uint8_t colorIndex);
            virtual void PutPixel(common::int32_t x, common::int32_t y, common::int32_t offset, common::uint8_t colorIndex);
            virtual void SwapBuffers();

            virtual void FillRectangle(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h,   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            virtual void DrawRectangleOutline(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h,   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            virtual void DrawCircle(common::uint32_t x, common::uint32_t y, common::uint32_t radius, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            virtual void DrawCircleQuadrant(common::uint32_t x, common::uint32_t y, common::uint32_t radius, common::uint8_t r, common::uint8_t g, common::uint8_t b, common::int32_t quadrant);
            virtual void DrawChar(char c, common::uint32_t x, common::uint32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b, common::int32_t vscale, common::int32_t hscale);
            virtual void DrawText(const char* string, common::uint32_t x, common::uint32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b, common::int32_t vscale, common::int32_t hscale);
            virtual void DrawLine(common::uint32_t x1, common::uint32_t y1, common::uint32_t x2, common::uint32_t y2, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            virtual void DrawBitmapImage(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h, common::uint8_t* data);
        };

    }
}

#endif