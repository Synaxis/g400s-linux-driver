//this is a rewrite/refactor from Matti 'cc' <ccr@tnsp.org>
// Change DPI and driver mode on Logitech "G400s Gaming Mouse"
// By Matti 'ccr' Hamalainen <ccr@tnsp.org>
//
// Based on https://bitbucket.org/extaliones/g400_hack/raw/b1a0f430dcb1c10991294447bf4f74a6acfff748/g400_hack.c
// by Przemek Aksamit <extaliones dot gmail.com>
// Compilation: gcc -o g400s_hack g400s_hack.c `pkg-config libusb-1.0 --cflags --libs`

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>


#define G400_VENDOR_ID  0x046d
#define G400_PRODUCT_ID 0xc24c


void errmsg(const char *str, int ret)
{
  printf("%s [%d: %s / %s]\n",
    str, ret,
    libusb_error_name(ret),
    libusb_strerror(ret));
}


int main(int argc, char *argv[])
{
  libusb_context *usb_ctx = NULL;
  libusb_device_handle *usb_handle = NULL;
  int usb_detached = 0;
  int usb_claimed  = 0;
  int ret, dpi_idx, dmode;
  char *arg;

  if (argc < 3)
  {
    printf(
      "Usage: %s <dpi> <mode>\n"
      "<dpi> is DPI sensitivity:\n"
      "   400|800|1800|3600\n"
      "\n"
      "<mode> is 'driver mode' value:\n"
      "   0 = 'driverless', special button events not sent to host\n"
      "   1 = driver mode, special button events sent to host\n"
      "\n", argv[0]);
    return 1;
  }

  // 0 => 400dpi, 1 => 800dpi, 2 => 1800dpi, 3 => 3600dpi
  arg = argv[1];
  switch (arg[0])
  {
    case '4': dpi_idx = 0; break;
    case '8': dpi_idx = 1; break;
    case '1': dpi_idx = 2; break;
    case '3': dpi_idx = 3; break;
    default:
      printf("Invalid DPI value '%s'.\n", arg);
      goto out;
  }

  arg = argv[2];
  dmode = atoi(arg);
  if (dmode != 0 && dmode != 1)
  {
    printf("Invalid index value '%s', must be 0 or 1.\n", arg);
    goto out;
  }

  // Initialize libusb and find device
  if ((ret = libusb_init(&usb_ctx)) != 0)
  {
    errmsg("LibUSB initialization failed!", ret);
    goto out;
  }
  
  usb_handle = libusb_open_device_with_vid_pid(usb_ctx, G400_VENDOR_ID, G400_PRODUCT_ID);
  if (usb_handle == NULL)
  {
    printf("Logitech G400 not found! (Do you have permissions to the usb devices? Try with sudo?)\n");
    goto out;
  }

  // Check if there is a driver attached
  if (libusb_kernel_driver_active(usb_handle, 1) == 1)
  {
    // Attempt to detach it temporarily
    if ((ret = libusb_detach_kernel_driver(usb_handle, 1)) != 0)
    {
      errmsg("Can't detach kernel driver.", ret);
      goto out;
    }
    usb_detached = 1;
  }

  // Claim the interface for our purposes
  if ((ret = libusb_claim_interface(usb_handle, 1)) != 0)
  {
    errmsg("Failed to claim interface.", ret);
    goto out;
  }
  usb_claimed = 1;

  // Send the data
  printf("Setting DPI index to %d, driver mode is %s.\n", dpi_idx, dmode ? "ENABLED" : "DISABLED");

  if ((ret = libusb_control_transfer(usb_handle,
    0x40, 2, 0x008e,
    (0x03 + dpi_idx) |     // The two lowest bits seem to be needed, results in error otherwise
    (dmode ? 0x80 : 0x00), // The 8th bit seems to control the driver mode
    0, 0, 1000)) != 0)
  {
    errmsg("Error writing to USB device!", ret);
    goto out;
  }

  printf("Successfully finished.\n");

  // Shut down
out:
  if (usb_handle != NULL)
  {
    if (usb_claimed)
      libusb_release_interface(usb_handle, 1);

    if (usb_detached)
      libusb_attach_kernel_driver(usb_handle, 1);

    libusb_close(usb_handle);
  }

  if (usb_ctx != NULL)
    libusb_exit(usb_ctx);

  return 0;
}
