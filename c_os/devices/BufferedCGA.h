#ifndef __BUFFEREDCGA_INCLUDE_H_
#define __BUFFEREDCGA_INCLUDE_H_

#include "devices/CGA.h"
#include "devices/Keyboard.h"
#include "lib/ScrollbackBuffer.h"

// NOTE: I added this file, I don't know if it will be used till the end but right know it's nice to have

// NOTE: I wanted to write to screen_buffer always and copy this to video mem to allow easier composition
//       of different screen elements, but this would disable printing before initialization or I would
//       have to replace print with unbuffered_print or something, which I don't like.
class BufferedCGA : public CGA {
private:
    ScrollbackBuffer* scrollback_buffer;  // Contains previous pages
    CGA::cga_page_t* screen_buffer;       // Contains the current page separately from the scrollback.
    bool initialized;                     // Don't do ScrollbackBuffer actions if not initialized

    BufferedCGA(const BufferedCGA&) = delete;

public:
    BufferedCGA() : CGA(), initialized(false), scrollback(0) {};
    ~BufferedCGA() {
        if (this->initialized) {
            delete this->scrollback_buffer;
            delete this->screen_buffer;
        }
    }

    unsigned char scrollback;  // The page that is displayed, public to enable page display

    void init(unsigned int pages);  // Scrollback needs to be initialized after memorymanagement
    void display_scrollback();      // Write the current_page to CGA memory
    void scroll_page_backward();    // Scroll up the page history
    void scroll_page_forward();     // Scroll down the page history (to the current page)

    void print(char* string, int n, unsigned char attrib = STD_ATTR) override;
    void scrollup() override;
    void clear() override;
};

#endif
