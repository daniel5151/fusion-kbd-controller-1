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

#include <libusb-1.0/libusb.h>
#include <stdint.h>

// This is the list of predefined modes.
// For custom config, this byte describes the config slot
typedef enum {
  MODE_STATIC = 0x00,
  MODE_BREATHING,
  MODE_WAVE,
  MODE_FADE_ON_KEYPRESS,
  MODE_MARQUEE,
  MODE_RIPPLE,
  MODE_FLASH_ON_KEYPRESS,
  MODE_NEON,
  MODE_RAINBOW_MARQUEE,
  MODE_RAINDROP,
  MODE_CIRCLE_MARQUEE,
  MODE_HEDGE,
  MODE_ROTATE,
} Mode;

typedef enum {
  COLOR_RED = 0x01,
  COLOR_GREEN,
  COLOR_YELLOW,
  COLOR_BLUE,
  COLOR_PURPLE,
  COLOR_AQUA,
  COLOR_WHITE,
} Color;

int set_mode(libusb_device_handle* handle,
             Mode mode,
             Color color,
             uint8_t brightness,
             uint8_t speed);

int set_custom_mode(libusb_device_handle* handle, uint8_t* data);
