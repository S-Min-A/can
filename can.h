int CAN_Init(int can_id);
void CAN_Read(int sockfd, int can_id, void *rx_buffer, uint8_t len);
void CAN_Write(int sockfd, int can_id, void *tx_buffer, uint8_t len);
