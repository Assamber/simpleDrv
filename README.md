# Simple Block Driver

Simple example of block driver. In Template dir you can find settigns for vscode c_cpp plugin. Change paths for you current one.

## Prepare
```console
sudo apt update
sudo apt upgrade
sudo apt install linux-headers-$(uname –r)
sudo apt install gcc
sudo apt install make
```
## Make Cmd
From Drv directory you can compile kernel headers and driver.
```console
make all
make install
make uninstall
make clean
```

## Tools
Show debug messages:
```console
sudo dmesg
```
Show installed kernel modules:
```console
sudo lsmod
cat /proc/devices
ls /dev
sudo fdisk -l
cat /proc/partition
```

## Useful Links

* [The Linux Kernel. Block Deivce Drivers](https://linux-kernel-labs.github.io/refs/heads/master/labs/block_device_drivers.html)
*  [Habr. Simple Block Device for Kernel 5.0](https://habr.com/ru/companies/veeam/articles/446148/)
* [Linux Device Drivers. Third Edition](https://bootlin.com/doc/books/ldd3.pdf)