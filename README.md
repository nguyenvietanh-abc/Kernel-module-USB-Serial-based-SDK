# Kernel-module-USB-Serial-based-SDK
kernel module
Cross compile:

export CROSS_COMPILE=aarch64-linux-gnu-  
export ARCH=arm64  
make  

Sau khi build kernel nếu đặt = y sẽ tự động trực tiếp insmod vào kernel
                     nếu đặt = m sẽ sinh my_serial_module.ko có thể insmod trực tiếp trên board nhúng.


Test mức độ hoạt động của kernel module my_serial_module.ko:
Kernel module chạy ở tầng user space (bên trong kernel)
test_serial.c hoạt động ở tầng user để giao tiếp thực tế với module qua thiết bị /dev/my_serial

