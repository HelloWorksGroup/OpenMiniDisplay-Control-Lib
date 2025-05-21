#include "omd_hid_lib.h"
#include <hidsdi.h>
#include <setupapi.h>
#include <stdio.h>

static HMODULE hHID = NULL;

int hid_init()
{
	hHID = LoadLibrary("hid.dll");
	return hHID != NULL;
}

void hid_cleanup()
{
	if (hHID) {
		FreeLibrary(hHID);
	}
}

HANDLE hid_open_device(WORD vendor_id, WORD product_id)
{
	GUID hid_guid;
	HidD_GetHidGuid(&hid_guid);

	HDEVINFO device_info = SetupDiGetClassDevs(&hid_guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (device_info == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	SP_DEVICE_INTERFACE_DATA interface_data;
	interface_data.cbSize = sizeof(interface_data);

	for (DWORD i = 0; SetupDiEnumDeviceInterfaces(device_info, NULL, &hid_guid, i, &interface_data); ++i) {
		DWORD size = 0;
		SetupDiGetDeviceInterfaceDetail(device_info, &interface_data, NULL, 0, &size, NULL);
		PSP_DEVICE_INTERFACE_DETAIL_DATA detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(size);
		detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		if (SetupDiGetDeviceInterfaceDetail(device_info, &interface_data, detail_data, size, NULL, NULL)) {
			HANDLE device = CreateFile(detail_data->DevicePath, GENERIC_READ | GENERIC_WRITE,
			                           FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
			if (device != INVALID_HANDLE_VALUE) {
				HIDD_ATTRIBUTES attrs;
				attrs.Size = sizeof(attrs);
				if (HidD_GetAttributes(device, &attrs)) {
					if (attrs.VendorID == vendor_id && attrs.ProductID == product_id) {
						free(detail_data);
						SetupDiDestroyDeviceInfoList(device_info);
						return device;
					}
				}
				CloseHandle(device);
			}
		}
		free(detail_data);
	}

	SetupDiDestroyDeviceInfoList(device_info);
	return NULL;
}

void hid_close_device(HANDLE device)
{
	if (device) {
		CloseHandle(device);
	}
}

int hid_write(HANDLE device, const unsigned char* data, size_t length)
{
	DWORD written = 0;
	if (!WriteFile(device, data, (DWORD)length, &written, NULL)) {
		return -1;
	}
	return (int)written;
}

int hid_read(HANDLE device, unsigned char* buffer, size_t length)
{
	DWORD read = 0;
	if (!ReadFile(device, buffer, (DWORD)length, &read, NULL)) {
		return -1;
	}
	return (int)read;
}
