#include "setup.h"

#include <stdio.h>

#include <libusb-1.0/libusb.h>

int fusionkbd_libusb_setup(libusb_context** ctx, libusb_device_handle** handle) {
  int err_libusb = 0;

  err_libusb = libusb_init(&*ctx);
  if (err_libusb < 0) {
    printf("libusb_init error %d\n", err_libusb);
    return -1;
  }

  *handle = libusb_open_device_with_vid_pid(*ctx, KBD_VID, KBD_PID);
  if (*handle == NULL) {
    printf("Failed to open device!\n");
    printf("Are you running as root?\n");
    libusb_exit(*ctx);
    return -1;
  }

  err_libusb = libusb_set_auto_detach_kernel_driver(*handle, 0);
  if (err_libusb < 0) {
    printf("Kernel ctrl driver auto detach failed.\n");
    goto exit;
  }
  err_libusb = libusb_set_auto_detach_kernel_driver(*handle, 3);
  if (err_libusb < 0) {
    printf("Kernel driver auto detach failed.\n");
    goto exit;
  }

  err_libusb = libusb_claim_interface(*handle, 0);
  if (err_libusb < 0) {
    printf("Failed to claim ctrl interface! %d\n", err_libusb);
    goto exit;
  }
  err_libusb = libusb_claim_interface(*handle, 3);
  if (err_libusb < 0) {
    printf("Failed to claim interface! %d\n", err_libusb);
    goto exit;
  }

exit:
  if (err_libusb < 0) fusionkbd_libusb_cleanup(*ctx, *handle);
  return err_libusb;
}

void fusionkbd_libusb_cleanup(libusb_context* ctx, libusb_device_handle* handle) {
  libusb_release_interface(handle, 0);
  libusb_release_interface(handle, 3);
  libusb_close(handle);
  libusb_exit(ctx);
}
