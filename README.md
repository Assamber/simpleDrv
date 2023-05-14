# Simple Block Driver

Simple example of block driver. It consist from one **block device driver** with RAM buffer and two user mode applications. **Writer** communicate with driver and translate data to file shared memory. **Viewer** read file shared memory and print it to console. Tested on *Ubuntu 22.04.2 LTS* \
In Template dir you can find settigns for vscode c_cpp plugin. Change paths for you current one. Installed linux header in prepare part need to be compiled.

## Prepare
```console
sudo apt update
sudo apt upgrade
sudo apt install linux-headers-$(uname –r)
sudo apt install gcc
sudo apt install make
```
## Make Cmd
From Drv directory you can compile kernel headers and driver. You can also install and uninstall it.
```console
make all
make install
make uninstall
make clean
```
In User directory you can compile two user mode application.

## Tools
Show debug messages:
```console
sudo dmesg
```
Show information about kernel module:
```console
sudo lsmod
cat /proc/devices
ls /dev
sudo fdisk -l
cat /proc/partition
blockdev --getsize64 /dev/DRiVeerDNA0
```

Show kernel version
```
lsb_release -a
```

## Useful Links

* [The Linux Kernel. Block Deivce Drivers](https://linux-kernel-labs.github.io/refs/heads/master/labs/block_device_drivers.html)
* [Linux Device Drivers. Third Edition](https://bootlin.com/doc/books/ldd3.pdf)
* [The Linux Kernel documentation](https://www.kernel.org/doc/html/latest/)
* [Habr. Simple Block Device for Kernel 5.0](https://habr.com/ru/companies/veeam/articles/446148/)
* [Linux kernel. Block I/O Layer](https://www.cs.cornell.edu/courses/cs4410/2021fa/assets/material/lecture24_blk_layer.pdf)
* [Bootlin. Block Device Drivers](https://bootlin.com/doc/legacy/block-drivers/block_drivers.pdf)
* [Elexir. Bootlin](https://elixir.bootlin.com/)