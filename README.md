# g400s-linux-driver
logitech g400s driver linux c 

* `git clone https://github.com/Synaxis/g400s-linux-driver && cd g400s-linux-driver
Requirements
*  `sudo apt-get install libusb-1.0-0-dev libusb-1.0 python3 libhidapi-libusb0 -y `

build and run with: 
 `gcc g400s.c  -o g400s `pkg-config libusb-1.0 --cflags --libs`

This is tested in Ubuntu 18.04 and 19.04Beta x64
