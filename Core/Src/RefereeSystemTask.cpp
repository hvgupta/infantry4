#include "RefereeSystemTask.hpp"

// #include "Interboard.hpp"
#include "RefereeClientUIItem.hpp"
#include "RefereeClientUIManager.hpp"
#include "RefereeClientUITemplates.hpp"
// #include "SuperCapaManager.hpp"
#include "math.h"
#include "task.h"

using namespace Core::Communication::RefereeSystemUI;

// debug watch
static volatile float refereeUserDebug[10] = {0};
// task variables
StaticTask_t xRefereeTaskTCB;
StackType_t uxRefereeTaskStack[4096];
// rx messages
CompetitionStatus competitionStatus;
RobotConfig robotConfig;
RefereePowerHeatMessage refereePowerHeatMessage;
RefereeShootingMessage refereeShootingMessage;

// static Core::Control::SuperCapacitor::CapacitorStatus spConfig;

bool hideGuide = false;

static StateIndicator cvStatus(20, 1170, 900, 5, 2, "COKETRASOP", 0);

// task
[[noreturn]] void refereeTask(void *param)
{
    // init
    if (registerMessage(&competitionStatus) < 0)
        assertFailed((uint8_t *)__FILE__, __LINE__);
    if (registerMessage(&robotConfig) < 0)
        assertFailed((uint8_t *)__FILE__, __LINE__);
    if (registerMessage(&refereePowerHeatMessage) < 0)
        assertFailed((uint8_t *)__FILE__, __LINE__);
    if (registerMessage(&refereeShootingMessage) < 0)
        assertFailed((uint8_t *)__FILE__, __LINE__);
    init();

    RefereeClientUIManager::setRobotID(robotConfig.getData().robotId);
    cvStatus.init();
    // cvStatus.show();
    vTaskDelay(1000);
    // task loop
    while (true)
    {
        // cvStatus.update(1);
        vTaskDelay(20);
    }
}