#include <stdio.h> // printf
#include <stdint.h> // uint8_t
#include <fcntl.h> // open
#include <unistd.h> // read, write
#include <string.h> // strcpy, memcpy
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/can/raw.h>

#include "can.h"

#define CAN_DEVICE "vcan1"

static int32_t sock;

int main(void)
{
    int32_t status = IO_CAN_SUCCESS;
    int32_t can_id = 0x123;
    uint8_t rx_buffer[1] = {0x00};

    CAN_Init();

    while (1)
    {
        status = IO_CAN_SUCCESS;
        rx_buffer[0] = 0x00;

        if ((status = CAN_Read(can_id, rx_buffer, 1)) < 0)
        {
            printf("CAN Error (Error code : %d)\n", status);
        }
        printf("rx : %d\n", rx_buffer[0]);

        sleep(1);
    }
}

int32_t CAN_Init(void)
{
    struct ifreq ifr;
    struct sockaddr_can addr;

    if ((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        return IO_CAN_ERROR_SOCKET;
    }

    strcpy(ifr.ifr_name, CAN_DEVICE);
    ioctl(sock, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    return IO_CAN_SUCCESS;
}

int32_t CAN_Read(int can_id, void *rx_buffer, uint8_t len)
{
    struct can_filter filter;
    struct can_frame frame;

    filter.can_id = can_id;
    filter.can_mask = CAN_SFF_MASK;
    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));

    if (sock < 0)
    {
        return IO_CAN_ERROR_INIT;
    }
    if (read(sock, &frame, sizeof(frame)) < 0)
    {
        return IO_CAN_ERROR_READ;
    }

    memcpy(rx_buffer, frame.data, len);

    return IO_CAN_SUCCESS;
}

int32_t CAN_Write(int can_id, void *tx_buffer, uint8_t len)
{
    struct can_frame frame;

    frame.can_id = can_id;
    frame.can_dlc = len;
    memcpy(frame.data, tx_buffer, len);

    if (sock < 0)
    {
        return IO_CAN_ERROR_INIT;
    }
    if (write(sock, &frame, sizeof(frame)) < 0)
    {
        return IO_CAN_ERROR_WRITE;
    }

    return IO_CAN_SUCCESS;
}
