#include "bsp_bmi088.h"
uint8_t command[2] = {0xFF,0xEE};
//uint8_t CMD = 0x00;
/*
创建多个陀螺仪结构体，包含发送命令变量以及接收变量
指定特定的串口对应该陀螺仪结构体
*/

void Set_SEND_DIV_H(struct Set_CMD_s *cmd, int Zero_Or_One)
{
    if (Zero_Or_One == 1)
        cmd->CMD |= (1 << 6);
    else if (Zero_Or_One == 0)
        cmd->CMD &= ~(1 << 6);
    else; // 给错咯
}

void Set_SEND_DIV_L(struct Set_CMD_s *cmd, int Zero_Or_One)
{
    if (Zero_Or_One == 1)
        cmd->CMD |= (1 << 5);
    else if (Zero_Or_One == 0)
        cmd->CMD &= ~(1 << 5);
    else; // 给错咯
}

void Set_SEND_Q(struct Set_CMD_s *cmd, int Set_Or_Not)
{
    if (Set_Or_Not == 1)
        cmd->CMD |= (1 << 4);
    else if (Set_Or_Not == 0)
        cmd->CMD &= ~(1 << 4);
    else; // 给错咯
}

void Set_SEND_TOTAL_YAW(struct Set_CMD_s *cmd, int Set_Or_Not)
{
    if (Set_Or_Not == 1)
        cmd->CMD |= (1 << 3);
    else if (Set_Or_Not == 0)
        cmd->CMD &= ~(1 << 3);
    else; // 给错咯
}

void Set_SEND_EULER_ANGLE(struct Set_CMD_s *cmd, int Set_Or_Not)
{
    if (Set_Or_Not == 1)
        cmd->CMD |= (1 << 2);
    else if (Set_Or_Not == 0)
        cmd->CMD &= ~(1 << 2);
    else; // 给错咯
}

void Set_CALIBRATE(struct Set_CMD_s *cmd, int Set_Or_Not)
{
    if (Set_Or_Not == 1)
        cmd->CMD |= (1 << 1);
    else if (Set_Or_Not == 0)
        cmd->CMD &= ~(1 << 1);
    else; // 给错咯
}

void Set_RESET(struct Set_CMD_s *cmd, int Set_Or_Not)
{
    if (Set_Or_Not == 1)
        cmd->CMD |= 1;
    else if (Set_Or_Not == 0)
        cmd->CMD &= ~1;
    else; // 给错咯
}



void Set_CMD_Init(Set_CMD_t *Set_CMD,UART_HandleTypeDef *huart)
{
    Set_CMD->CMD = 0x00;
    Set_CMD->huart = huart;

    Set_CMD->PITCH = 0;
    Set_CMD->YAW = 0;
    Set_CMD->ROLL = 0;
    Set_CMD->TOTAL_YAW = 0;
    Set_CMD->Q[0] = 0;
    Set_CMD->Q[1] = 0;
    Set_CMD->Q[2] = 0;
    Set_CMD->Q[3] = 0;



    Set_CMD->Set_SEND_DIV_H = Set_SEND_DIV_H;
    Set_CMD->Set_SEND_DIV_L = Set_SEND_DIV_L;
    Set_CMD->Set_SEND_Q = Set_SEND_Q;
    Set_CMD->Set_SEND_TOTAL_YAW = Set_SEND_TOTAL_YAW;
    Set_CMD->Set_SEND_EULER_ANGLE = Set_SEND_EULER_ANGLE;
    Set_CMD->Set_CALIBRATE = Set_CALIBRATE;
    Set_CMD->Set_RESET = Set_RESET;
}

Set_CMD_t Set_CMD;

void UART_Init(void)
{
    huart1.Instance = USART1;
  huart1.Init.BaudRate = 460800; // 设置波特率为460800
  huart1.Init.WordLength = UART_WORDLENGTH_8B; // 设置数据位为8
  huart1.Init.StopBits = UART_STOPBITS_1; // 设置停止位为1
  huart1.Init.Parity = UART_PARITY_NONE; // 设置校验位为无
  huart1.Init.Mode = UART_MODE_TX_RX; // 设置为发送和接收模式
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 关闭硬件流控制
  huart1.Init.OverSampling = UART_OVERSAMPLING_16; // 设置过采样为16
  HAL_UART_Init(&huart1);
}

void Send_Header (UART_HandleTypeDef *huart)
{
    HAL_UART_Transmit(huart, command[0], 1, HAL_MAX_DELAY);
}
void Send_Footer (UART_HandleTypeDef *huart)
{
    HAL_UART_Transmit(huart, command[1], 1, HAL_MAX_DELAY);
}

void Send_CMD (Set_CMD_t *Set_CMD)
{
    Send_Header(Set_CMD->huart);
    HAL_UART_Transmit(Set_CMD->huart, Set_CMD->CMD, 1, HAL_MAX_DELAY);
    Send_Footer(Set_CMD->huart);
}

void Recive_CMD(Set_CMD_t *Set_CMD)
{
    uint8_t recive_buffer[36]; // 接收缓冲区，大小为36字节
    HAL_UART_Receive(Set_CMD->huart, recive_buffer, 36, HAL_MAX_DELAY); // 接收36字节数据
    /*校验帧尾*/
    if (recive_buffer[35] == 0x7F && recive_buffer[34] == 0x80 && recive_buffer[33] == 0x00 & recive_buffer[32] == 0x00)
    {
        Set_CMD->PITCH = recive_buffer[0] << 24 | recive_buffer[1] << 16 | recive_buffer[2] << 8 | recive_buffer[3];
        Set_CMD->YAW   = recive_buffer[4] << 24 | recive_buffer[5] << 16 | recive_buffer[6] << 8 | recive_buffer[7];
        Set_CMD->ROLL  = recive_buffer[8] << 24 | recive_buffer[9] << 16 | recive_buffer[10] << 8 | recive_buffer[11];
        Set_CMD->TOTAL_YAW = recive_buffer[12] << 24 | recive_buffer[13] << 16 | recive_buffer[14] << 8 | recive_buffer[15];
        Set_CMD->Q[0] = recive_buffer[16] << 24 | recive_buffer[17] << 16 | recive_buffer[18] << 8 | recive_buffer[19];
        Set_CMD->Q[1] = recive_buffer[20] << 24 | recive_buffer[21] << 16 | recive_buffer[22] << 8 | recive_buffer[23];
        Set_CMD->Q[2] = recive_buffer[24] << 24 | recive_buffer[25] << 16 | recive_buffer[26] << 8 | recive_buffer[27];
        Set_CMD->Q[3] = recive_buffer[28] << 24 | recive_buffer[29] << 16 | recive_buffer[30] << 8 | recive_buffer[31];
    }
} 