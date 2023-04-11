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
#include "gpio.h"
#include "main.h"
#include "RosComm.hpp"
#include "task.h"
#include "tim.h"
#include "usart.h"
#include "DJIMotor.hpp"


StackType_t uxBlinkTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xBlinkTaskTCB;

// #define HZ2PSC(x) (144000000/5000/(x))
inline uint16_t HZ2PSC(float freq) { return (int16_t)(144000000.0f/5000.0f/freq); }

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

void processGimbal(uint8_t* data, uint16_t len)
{
    static volatile int here = 0;
    here += 1;
}

void processShooter(uint8_t* data, uint16_t len)
{
    static volatile int here = 0;
    here += 1;
}
StackType_t comTestStack[configMINIMAL_STACK_SIZE];
StaticTask_t comTestTCB;
void com(void *)
{
    using namespace Core::Communication;
    static RosComm::ChassisStatus chassisStatus;
    static RosComm::FrameHeader chassisHeader;
    chassisHeader.dataLen = sizeof(RosComm::ChassisStatus);
    chassisHeader.protocolID = RosComm::CHASSIS_MSG;
    chassisStatus.wVel = 114.0;
    chassisStatus.xVel = 514.0;
    chassisStatus.yVel = 618.3;

    static RosComm::GimbalStatus gimbalStatus;
    static RosComm::FrameHeader gimbalHeader;
    gimbalStatus.q[2] = 3;
    gimbalHeader.dataLen = sizeof(RosComm::GimbalStatus);
    gimbalHeader.protocolID = RosComm::GIMBAL_MSG;

    static RosComm::CVMode cvMode;
    static RosComm::FrameHeader cvHeader;
    cvHeader.dataLen = sizeof(RosComm::CVMode);
    cvHeader.protocolID = RosComm::CVMODE_MSG;

    cvMode.color = RosComm::CVMode::RED;
    cvMode.curCVMode = Core::Communication::RosComm::CVMode::MANULA;
    cvMode.bulletSpeed = 12.34f;

    registerProcessFunction(RosComm::GIMBAL_CMD, processGimbal);
    registerProcessFunction(RosComm::SHOOTER_CMD, processShooter);

    DJIMotor::DJIMotor& motor = DJIMotor::getMotor(0x201);
    motor.setOutputCurrent(-10000);
    while(1)
    {
        RosComm::transmit(chassisHeader, reinterpret_cast<uint8_t*>(&chassisStatus));
        RosComm::transmit(gimbalHeader, reinterpret_cast<uint8_t*>(&gimbalStatus));
        RosComm::transmit(cvHeader, reinterpret_cast<uint8_t*>(&cvMode));
        DJIMotor::sendMotorGroup(0);
        vTaskDelay(1);
    }
}


/**
 * @brief Create user tasks
 */
void startUserTasks()
{
    using namespace Core::Communication;
    RosComm::init();
    // DJIMotor::init();
    xTaskCreateStatic(blink, "blink", configMINIMAL_STACK_SIZE, NULL, 0, uxBlinkTaskStack, &xBlinkTaskTCB);
    xTaskCreateStatic(com, "com", configMINIMAL_STACK_SIZE, NULL, 0, comTestStack, &comTestTCB);
}
