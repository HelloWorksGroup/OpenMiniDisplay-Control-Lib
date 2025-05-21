CC = gcc
CFLAGS = -Wall -Wextra -shared -mwindows -O2
LDFLAGS = -lhid -lsetupapi

BUILD_DIR = build

SHELL := powershell.exe
.SHELLFLAGS := -NoLogo -NoProfile -Command

default: all

all: omd_hid_lib.dll
	rm omd_hid_lib.o
	mkdir -Force $(BUILD_DIR) | Out-Null
	mv -Force omd_hid_lib.dll ./$(BUILD_DIR)

omd_hid_lib.dll: omd_hid_lib.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

omd_hid_lib.o: omd_hid_lib.c omd_hid_lib.h
	$(CC) -c -o $@ $<

clean:
	rm $(BUILD_DIR) -r -force

format:
	astyle --options=.astylerc *.c *.h
