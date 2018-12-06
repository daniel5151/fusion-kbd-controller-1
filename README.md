# fusion-kbd-controller

This project is a tiny userspace binary that allows you to configure the RGB
Fusion keyboard of the Gigabyte AERO 15X using libusb.

You can switch between the built-in presets, and upload custom configurations!

## Building

Ensure libusb and libusb-dev are installed

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Flag handling isn't too robust yet, but you can get an idea of usage by just
running `fusion_kbd` on it's own, and reading the usage.

cfg files are currently raw binary corresponding to the USB payload sent to the
keyboard.

Root privileges are required, since the tool has to temporarily unbinds the USB
device from the kernel module.

## Current state

It seems to work, but more testing and functionality work is required.

## Disclaimer

It's possible to brick your keyboard when sending bogus values over the wire!
While it seems to work fine for me, use this softawre at your own risk!

(that said, a "bricked" keyboard can usually be fixed with a reboot. Unless you
really mess it up, in which case, gg)