#!/bin/bash

NC='\033[0m'
CYAN='\033[0;36m'
sudo rm -rf /dev/heartydev
sudo rm -rf /dev/ioctl
echo -e "${CYAN}[sudo rmmod heartydev]${NC}"
sudo rmmod heartydev
echo -e "${CYAN}[rm heartydev.ko heartydev.mod.c heartydev.o main.o modules.order heartydev.mod heartydev.mod.o Module.symvers]${NC}"
sudo rm heartydev.ko heartydev.mod.c heartydev.o main.o modules.order heartydev.mod heartydev.mod.o Module.symvers
echo -e "${CYAN}[make]${NC}"
make
echo -e "${CYAN}[sudo insmod heartydev.ko]${NC}"
sudo insmod heartydev.ko
# echo -e "${CYAN}[cat /proc/modules | grep heartydev]${NC}"
# cat /proc/modules | grep heartydev
# echo -e "${CYAN}[cat /proc/devices | grep heartydev]${NC}"
# cat /proc/devices | grep heartydev
echo -e "${CYAN}[tail /var/log/syslog]${NC}"
tail /var/log/syslog
echo -e "${CYAN}[mknod]${NC}"
MAJOR=$((16#`stat -c %t /dev/heartydev`))
sudo mknod /dev/ioctl c $MAJOR 0
echo -e "${CYAN}[echo 'Hello, Heartydev!' > /dev/heartydev]${NC}"
echo 'Hello, Heartydev!' > /dev/heartydev
echo -e "${CYAN}[cat /dev/heartydev]${NC}"
cat /dev/heartydev
echo -e "${CYAN}[gcc userspace_ioctl.c -o userspace_ioctl]${NC}"
gcc userspace_ioctl.c -o userspace_ioctl
echo -e "${CYAN}[sudo ./userspace_ioctl]${NC}"
sudo ./userspace_ioctl
rm userspace_ioctl
echo -e "${CYAN}[cat /dev/heartydev]${NC}"
cat /dev/heartydev
