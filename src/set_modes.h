// Copyright (c) 2018 Martin Koppehel <martin@embedded.enterprises>

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <stdint.h>

#include <libusb-1.0/libusb.h>

// This is the list of predefined modes.
// For custom config, this byte describes the config slot
#define PRESET_STATIC            0x01
#define PRESET_BREATHING         0x02
#define PRESET_WAVE              0x03
#define PRESET_FADE_ON_KEYPRESS  0x04
#define PRESET_MARQUEE           0x05
#define PRESET_RIPPLE            0x06
#define PRESET_FLASH_ON_KEYPRESS 0x07
#define PRESET_NEON              0x08
#define PRESET_RAINBOW_MARQUEE   0x09
#define PRESET_RAINDROP          0x0a
#define PRESET_CIRCLE_MARQUEE    0x0b
#define PRESET_HEDGE             0x0c
#define PRESET_ROTATE            0x0d

#define COLOR_RAND   0x00
#define COLOR_RED    0x01
#define COLOR_GREEN  0x02
#define COLOR_YELLOW 0x03
#define COLOR_BLUE   0x04
#define COLOR_ORANGE 0x05
#define COLOR_PURPLE 0x06
#define COLOR_WHITE  0x07

int set_mode(libusb_device_handle* handle,
             uint8_t mode,
             uint8_t color,
             uint8_t brightness,
             uint8_t speed);

int set_custom_mode(libusb_device_handle* handle,
                    uint8_t* data,
                    uint8_t brightness);
