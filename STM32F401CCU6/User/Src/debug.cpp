/**
 * @file debug.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 使用USB虚拟串口模拟的调试窗口。
 * @version 0.1
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "debug.h"
#include "usbd_cdc_if.h"
#include "freertos.h"
#include "cmsis_os.h"

uint8_t RxDataBuffer[BUFFER_SIZE];


void Debug_Main(void) {
    while (1) {
        ;
        osDelay(1);
    }
}

HAL_StatusTypeDef Debug_Init(void) {
    usb_printf("Debug section initializing\n");
    return HAL_OK;
}

HAL_StatusTypeDef Debug_ProcessCommand(uint8_t *cmd) {
    switch (cmd[0]) {
        default:
            return HAL_OK;
    }
    return HAL_ERROR;
}

