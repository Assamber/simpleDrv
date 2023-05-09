# Simple Driver

Simple example of block driver

## Load Driver
```console
sudo insmod Drv.ko
```
## Unload Driver
```console
sudo rmmod Drv
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