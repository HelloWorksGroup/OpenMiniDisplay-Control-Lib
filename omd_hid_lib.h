#ifndef HID_COMM_H
#define HID_COMM_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) int hid_init();
__declspec(dllexport) void hid_cleanup();
__declspec(dllexport) HANDLE hid_open_device(WORD vendor_id, WORD product_id);
__declspec(dllexport) void hid_close_device(HANDLE device);
__declspec(dllexport) int hid_write(HANDLE device, const unsigned char* data, size_t length);
__declspec(dllexport) int hid_read(HANDLE device, unsigned char* buffer, size_t length);

#ifdef __cplusplus
}
#endif

#endif
