typedef struct {
    uint8_t CMD;
    UART_HandleTypeDef *huart//指定发送接收串口
    int PITCH;
    int YAW;
    int ROLL;
    int TOTAL_YAW;
    float Q[4];//四元数
    void (*Set_SEND_DIV_H)(struct Set_CMD_s *, int);
    void (*Set_SEND_DIV_L)(struct Set_CMD_s *, int);
    void (*Set_SEND_Q)(struct Set_CMD_s *, int);
    void (*Set_SEND_TOTAL_YAW)(struct Set_CMD_s *, int);
    void (*Set_SEND_EULER_ANGLE)(struct Set_CMD_s *, int);
    void (*Set_CALIBRATE)(struct Set_CMD_s *, int);
    void (*Set_RESET)(struct Set_CMD_s *, int);
} Set_CMD_t;

void Set_CMD_Init(Set_CMD_t *Set_CMD,UART_HandleTypeDef *huart);
void Send_CMD (Set_CMD_t *Set_CMD);
void Recive_CMD(Set_CMD_t *Set_CMD);
