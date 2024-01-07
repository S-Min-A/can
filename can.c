#include <stdio.h> // printf
#include <stdint.h> // uint8_t
#include <fcntl.h> // open
#include <unistd.h> // read, write
#include <string.h> // strcpy, memcpy
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/can/raw.h>

#include "can.h"

#define CAN_DEVICE "can0"

int main(void)
{
    int sock;
    uint8_t tx_buffer[1] = {0x00};
    uint8_t rx_buffer[1] = {0x00};

    sock = CAN_Init(0x000);

    while (1)
    {
        CAN_Write(sock, 0x000, tx_buffer, 1);
        printf("tx : %d\n", tx_buffer[0]);
        CAN_Read(sock, 0x000, rx_buffer, 1);
        printf("rx : %d\n", rx_buffer[0]);
        sleep(1);
    }
}

int CAN_Init(int can_id) 
{
    int sock;
    struct ifreq ifr;
    struct sockaddr_can addr;
    struct can_filter filter;

    if ((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        printf("Socket error!\n");
    }

    strcpy(ifr.ifr_name, CAN_DEVICE);
    ioctl(sock, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    filter.can_id = can_id;
    filter.can_mask = CAN_SFF_MASK;
    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));

    return sock;
}

void CAN_Read(int sock, int can_id, void *rx_buffer, uint8_t len)
{
    struct can_frame frame;

    if (read(sock, &frame, sizeof(frame)) < 0)
    {
        printf("Read error!\n");
    }

    memcpy(rx_buffer, frame.data, len);
}

void CAN_Write(int sock, int can_id, void *tx_buffer, uint8_t len)
{
    struct can_frame frame;

    frame.can_id = can_id;
    frame.can_dlc = len;
    memcpy(frame.data, tx_buffer, len);

    if (write(sock, &frame, sizeof(frame)) < 0)
    {
        printf("Write error!\n");
    }
}
