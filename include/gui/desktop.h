#ifndef __SWIFTOS__GUI__DESKTOP_H
#define __SWIFTOS__GUI__DESKTOP_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace swiftos
{
    namespace gui
    {

        class Desktop : public CompositeWidget, public swiftos::drivers::MouseEventHandler
        {
        protected:
            common::uint32_t MouseX;
            common::uint32_t MouseY;

        public:
            Desktop(common::int32_t w, common::int32_t h,
                common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Desktop();

            void Draw(common::GraphicsContext* gc);

            void OnMouseDown(swiftos::common::uint8_t button);
            void OnMouseUp(swiftos::common::uint8_t button);
            void OnMouseMove(int x, int y);
        };

    }
}


#endif