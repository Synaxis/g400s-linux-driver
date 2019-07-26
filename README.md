# g400s-linux-driver
logitech g400s driver linux c 

* git clone https://github.com/Synaxis/g400s-linux-driver && cd g400s-linux-driver
Requirements
*  `sudo apt-get install libusb-1.0-0-dev libusb-1.0 -y `

build with: 
 gcc g400s.c -o g400s `pkg-config --libs --cflags libusb-1.0`

This is tested in Ubuntu 18.04 and 19.04Beta x64
