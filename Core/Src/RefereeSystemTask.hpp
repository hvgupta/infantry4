#pragma once
#include "RefereeSystemComm.hpp"
#include "stdint.h"
#include "string.h"

using namespace Core::Communication::RefereeSystem;

extern StaticTask_t xRefereeTaskTCB;
extern StackType_t uxRefereeTaskStack[4096];

[[noreturn]] void refereeTask(void *param);

class CompetitionStatus : public RefereeSystemMessage
{
   public:
    CompetitionStatus() : RefereeSystemMessage(0x0001, sizeof(RefereeGameStatus), 1) {}

    typedef struct
    {
        uint8_t gameType : 4;
        uint8_t gameProgress : 4;
        uint16_t stageRemainTime;
        uint64_t syncTimeStamp;
    } __attribute__((__packed__)) RefereeGameStatus;

    void decode(uint8_t *payload) override { memcpy((uint8_t *)&data, payload, sizeof(RefereeGameStatus)); }

    const RefereeGameStatus &getData() const { return data; }

   private:
    RefereeGameStatus data;
};

class RobotConfig : public RefereeSystemMessage
{
   public:
    RobotConfig() : RefereeSystemMessage(0x0201, sizeof(RefereeRobotStatus), 10) {}

    typedef struct
    {
        uint8_t robotId;
        uint8_t robotLevel;
        uint16_t remainHp;
        uint16_t maxHp;
        uint16_t shooterId117mmCoolingRate;
        uint16_t shooterId117mmCoolingLimit;
        uint16_t shooterId117mmSpeedLimit;
        uint16_t shooterId217mmCoolingRate;
        uint16_t shooterId217mmCoolingLimit;
        uint16_t shooterId217mmSpeedLimit;
        uint16_t shooterId142mmCoolingRate;
        uint16_t shooterId142mmCoolingLimit;
        uint16_t shooterId142mmSpeedLimit;
        uint16_t chassisPowerLimit;
        uint8_t mainsPowerGimbalOutput : 1;
        uint8_t mainsPowerChassisOutput : 1;
        uint8_t mainsPowerShooterOutput : 1;
    } __attribute__((__packed__)) RefereeRobotStatus;

    bool getId = false;

    void decode(uint8_t *payload) override
    {
        memcpy((uint8_t *)&data, payload, sizeof(RefereeRobotStatus));
        getId = true;
    }

    const RefereeRobotStatus &getData() const { return data; }

   private:
    RefereeRobotStatus data;
};

class RefereePowerHeatMessage : public RefereeSystemMessage
{
   public:
    RefereePowerHeatMessage() : RefereeSystemMessage(0x0202, sizeof(RefereePowerHeatMessageData), 50) {}

    typedef struct
    {
        uint16_t chassisVolt;
        uint16_t chassisCurrent;
        float chassisPower;
        uint16_t chassisPowerBuffer;
        uint16_t shooterId117mmCoolingHeat;
        uint16_t shooterId217mmCoolingHeat;
        uint16_t shooterId142mmCoolingHeat;
    } __attribute__((__packed__)) RefereePowerHeatMessageData;

    void decode(uint8_t *payload) override { memcpy((uint8_t *)&data, payload, sizeof(RefereePowerHeatMessageData)); }

    const RefereePowerHeatMessageData &getData() const { return data; }

   private:
    RefereePowerHeatMessageData data;
};

class RefereeShootingMessage : public RefereeSystemMessage
{
   public:
    RefereeShootingMessage() : RefereeSystemMessage(0x0207, sizeof(RefereeShootingMessageData), 0) {}

    // the datasheet is wrong?
    typedef struct
    {
        uint8_t bulletType;
        uint8_t shooterId;
        uint8_t bulletFreq;
        float bulletSpeed;
    } __attribute__((__packed__)) RefereeShootingMessageData;

    void decode(uint8_t *payload) override
    {
        memcpy((uint8_t *)&data, payload, sizeof(RefereeShootingMessageData));
        //        shootingCount++;
    }

    const RefereeShootingMessageData &getData() const { return data; }
    //    int getShootingCount() const { return shootingCount; }

   private:
    RefereeShootingMessageData data;
    //    int shooter1Count = 0;
    //    int shooter2Count = 0;
};

extern CompetitionStatus competitionStatus;
extern RobotConfig robotConfig;
extern RefereePowerHeatMessage refereePowerHeatMessage;
extern RefereeShootingMessage refereeShootingMessage;