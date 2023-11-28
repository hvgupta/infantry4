/**
 * @file UserTask.cpp
 * @author JIANG Yicheng  RM2023 (EthenJ@outlook.sg)
 * @brief Create user tasks with cpp support
 * @version 0.1
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2022
 */

#include "FreeRTOS.h"
#include "gpio.h"
#include "main.h"
#include "task.h"
#include "MF9015.hpp"

StackType_t uxBlinkTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xBlinkTaskTCB;

void blink(void *pvPara)
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
    using namespace Core::Drivers;
    MF9015 &motor1 = MF9015::getMotor(0x141);
    motor1.setCurrentLimit(2000);

    while (true)
    {
        motor1.setOutput(2000);
        // HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
        // HAL_GPIO_TogglePin(LASER_GPIO_Port, LASER_Pin);
        MF9015::sendMotorGroup();
        vTaskDelay(1);
    }
}

/**
 * @brief Create user tasks
 */
void startUserTasks()
{
    using namespace Core::Drivers;
    CANManager::managers[0].init(&hcan1);
    MF9015::init();
    xTaskCreateStatic(blink, "blink", configMINIMAL_STACK_SIZE, NULL, 0, uxBlinkTaskStack, &xBlinkTaskTCB);
}
