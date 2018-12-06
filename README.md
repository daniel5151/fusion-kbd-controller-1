# fusion-kbd-controller

This project is a tiny userspace binary that allows you to configure the RGB
Fusion keyboard of the Gigabyte AERO 15X using libusb.

The initial protocol reverse-engineering was done by martin31821. This repo
builds on top of his initial [fusion-kbd-controller](https://github.com/martin31821/fusion-kbd-controller)
project.

## Building

Ensure libusb and libusb-dev are installed

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

TODO

Root privileges are required, since the tool has to temporarily unbinds the USB
device from the kernel module.

## Current state

It seems to work, but more testing and functionality work is required.

## Disclaimer

It's possible to brick your keyboard when sending bogus values here.
You should be safe when using the high level `set_mode` and `set_custom_mode`
functions.
