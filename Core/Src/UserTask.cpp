/**
 * @file UserTask.cpp
 * @author JIANG Yicheng  RM2023 (EthenJ@outlook.sg)
 * @brief Create user tasks with cpp support
 * @version 0.1
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2022
 */
#include <cstdint>

#include "FreeRTOS.h"
#include "RefereeClientUIManager.hpp"
#include "RefereeSystemTask.hpp"
#include "RosComm.hpp"
#include "gpio.h"
#include "main.h"
#include "task.h"
#include "tim.h"
#include "usart.h"

// #include "DJIMotor.hpp"

StackType_t uxBlinkTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xBlinkTaskTCB;

// #define HZ2PSC(x) (144000000/5000/(x))
inline uint16_t HZ2PSC(float freq) { return (int16_t)(144000000.0f / 5000.0f / freq); }

void buzzerOn(uint16_t psc, uint16_t comp)
{
    __HAL_TIM_PRESCALER(&htim9, psc);
    __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_2, comp);
}
void buzzerOff(void) { __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_2, 0); }
void playNote(float freq, uint32_t duration, uint16_t intensity)
{
    buzzerOn(HZ2PSC(freq), intensity);
    vTaskDelay(duration);
    buzzerOff();
}

void blink(void *pvPara)
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
    HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);

    while (true)
    {
        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        vTaskDelay(500);
    }
}

void processGimbal(uint8_t *data, uint16_t len)
{
    static volatile int here = 0;
    here += 1;
}

void processShooter(uint8_t *data, uint16_t len)
{
    static volatile int here = 0;
    here += 1;
}
StackType_t comTestStack[configMINIMAL_STACK_SIZE];
StaticTask_t comTestTCB;
void com(void *)
{
    while (1)
        ;
}

/**
 * @brief Create user tasks
 */
void startUserTasks()
{
    using namespace Core::Communication;
    // RosComm::init();
    RefereeSystemUI::initUI();
    xTaskCreateStatic(refereeTask, "referee communication", 4096, NULL, 8, uxRefereeTaskStack, &xRefereeTaskTCB);
    xTaskCreateStatic(blink, "blink", configMINIMAL_STACK_SIZE, NULL, 0, uxBlinkTaskStack, &xBlinkTaskTCB);
    xTaskCreateStatic(com, "com", configMINIMAL_STACK_SIZE, NULL, 0, comTestStack, &comTestTCB);
}
