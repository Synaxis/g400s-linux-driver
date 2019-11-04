# g400s-linux-driver
Logitech G400s driver for Linux, written in C.

* ` git clone https://github.com/Synaxis/g400s-linux-driver && cd g400s-linux-driver `

Requirements
*  ` sudo apt-get install libusb-1.0-0-dev libusb-1.0 -y `

Build with: 
gcc `` g400s.c -o g400s `pkg-config --libs --cflags libusb-1.0` ``

This is tested in Ubuntu 18.04, 19.04 Beta and 19.10 (x64).
