#define IO_CAN_SUCCESS                      0
#define IO_CAN_ERROR                       -1
#define IO_CAN_ERROR_SOCKET                -2
#define IO_CAN_ERROR_INIT                  -3
#define IO_CAN_ERROR_WRITE                 -4
#define IO_CAN_ERROR_READ                  -5

int32_t CAN_Init(void);
int32_t CAN_Read(int can_id, void *rx_buffer, uint8_t len);
int32_t CAN_Write(int can_id, void *tx_buffer, uint8_t len);
