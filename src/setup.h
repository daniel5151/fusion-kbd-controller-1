#include <libusb-1.0/libusb.h>

#define KBD_VID 0x1044
#define KBD_PID 0x7a39

int  fusionkbd_libusb_setup(libusb_context** ctx, libusb_device_handle** handle);
void fusionkbd_libusb_cleanup(libusb_context* ctx, libusb_device_handle* handle);
