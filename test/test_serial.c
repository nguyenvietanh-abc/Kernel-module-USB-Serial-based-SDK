#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char write_buf[256] = "RADAR_CMD: request status";
    char read_buf[256] = {0};

    fd = open("/dev/my_serial", O_RDWR);
    if (fd < 0) {
        perror("Failed to open /dev/my_serial");
        return 1;
    }

    write(fd, write_buf, strlen(write_buf));
    printf("Đã gửi lệnh test Radar đến kernel module\n");

    read(fd, read_buf, sizeof(read_buf) - 1);
    printf("Nhận được từ module (decoded): %s\n", read_buf);

    close(fd);
    return 0;
}