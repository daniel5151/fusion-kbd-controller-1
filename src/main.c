#include <stdio.h>

#include <assert.h>
#include <errno.h>
#include <libusb-1.0/libusb.h>
#include <string.h>
#include <unistd.h>

#include "set_modes.h"

#define KBD_VID 0x1044
#define KBD_PID 0x7a39

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: %s mode [modeparams]...\n", argv[0]);
    return -1;
  }

  // set-up libusb

  libusb_context* ctx = NULL;

  int err_libusb = 0;
  int exitcode = 0;

  err_libusb = libusb_init(&ctx);
  if (err_libusb < 0) {
    printf("libusb_init error %d\n", err_libusb);
    return 1;
  }

  libusb_device_handle* handle = NULL;
  handle = libusb_open_device_with_vid_pid(ctx, KBD_VID, KBD_PID);
  if (handle == NULL) {
    printf("Failed to open device!\n");
    printf("Are you running as root?\n");
    libusb_exit(ctx);
    return 1;
  }
  if (libusb_set_auto_detach_kernel_driver(handle, 0) < 0) {
    printf("Kernel ctrl driver auto detach failed.\n");
    goto exit;
  }
  if (libusb_set_auto_detach_kernel_driver(handle, 3) < 0) {
    printf("Kernel driver auto detach failed.\n");
    goto exit;
  }

  err_libusb = libusb_claim_interface(handle, 0);
  if (err_libusb < 0) {
    printf("Failed to claim ctrl interface! %d\n", err_libusb);
    exitcode = 4;
    goto exit;
  }
  err_libusb = libusb_claim_interface(handle, 3);
  if (err_libusb < 0) {
    printf("Failed to claim interface! %d\n", err_libusb);
    exitcode = 2;
    goto exit;
  }

  // great! we have a proper libusb handle to the keyboard!

  // TODO: add cli support for built-in modes

  // arg-parsing
  char* mode = argv[1];
  if (strcmp(mode, "custom") == 0) {
    if (argc < 3) {
      printf("Usage: %s custom file\n", argv[0]);
      exitcode = -1;
      goto exit;
    }
    // Custom mode
    FILE* fd = fopen(argv[2], "rb");
    if (!fd) {
      printf("fopen(%s) failed: %s\n", argv[2], strerror(errno));
      exitcode = -1;
      goto exit;
    }

    // see data.inc for data-format
    #include "data.inc" // uint8_t m_blank_data[512];

    size_t result = fread(m_blank_data, 512, 1, fd);
    (void) result;
    fclose(fd);

    err_libusb = set_custom_mode(handle, m_blank_data);
    if (err_libusb < 0) {
      printf("Failed to set custom mode!\n");
      exitcode = -1;
      goto exit;
    }
    exitcode = -1;
    goto exit;
  }

exit:
  libusb_release_interface(handle, 0);
  libusb_release_interface(handle, 3);
  libusb_close(handle);
  libusb_exit(ctx);
  return exitcode;
}
