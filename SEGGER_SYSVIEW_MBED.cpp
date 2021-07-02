#include "SEGGER_SYSVIEW.h"
#include "rtx_os.h"
#include <string.h>
#include <stdlib.h>
#include <chrono>
#include "Kernel.h"

// SystemcoreClock can be used in most CMSIS compatible projects.
extern unsigned int SystemCoreClock;

// The application name to be displayed in SystemViewer
#define SYSVIEW_APP_NAME "Mbed OS Application"

// The target device name
#define SYSVIEW_DEVICE_NAME "Mbed OS Device"

// Frequency of the timestamp. Must match SEGGER_SYSVIEW_Conf.h
#define SYSVIEW_TIMESTAMP_FREQ (SystemCoreClock)

// System Frequency. SystemcoreClock is used in most CMSIS compatible projects.
#define SYSVIEW_CPU_FREQ (SystemCoreClock)

// The lowest RAM address used for IDs (pointers)
#define SYSVIEW_RAM_BASE (0x20000000)

static void sendTaskList(void)
{
    static osThreadId_t threads[32];

    osKernelLock();
    auto thread_n = osThreadEnumerate(threads, sizeof(threads) / sizeof(osThreadId_t));

    for (uint32_t i = 0; i < thread_n; i++)
    {
        osRtxThread_t *thread = (osRtxThread_t *)threads[i];

        SEGGER_SYSVIEW_TASKINFO info;
        memset(&info, 0, sizeof(info));
        info.TaskID = (unsigned long)threads[i];
        info.sName = thread->name;
        info.Prio = thread->priority;
        info.StackBase = (uint32_t)thread->stack_mem;
        info.StackSize = thread->stack_size;

        SEGGER_SYSVIEW_SendTaskInfo(&info);
    }
    osKernelUnlock();
}

static uint64_t getTime()
{
    using namespace std::chrono;
    auto now_ms = time_point_cast<microseconds>(rtos::Kernel::Clock::now());
    return now_ms.time_since_epoch().count();
}

const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI = {
    getTime,
    sendTaskList,
};

static void sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=" SYSVIEW_APP_NAME ",D=" SYSVIEW_DEVICE_NAME);
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}

void SEGGER_SYSVIEW_Conf(void)
{
    SEGGER_SYSVIEW_Init(SYSVIEW_TIMESTAMP_FREQ, SYSVIEW_CPU_FREQ,
                        &SYSVIEW_X_OS_TraceAPI, sendSystemDesc);
    SEGGER_SYSVIEW_SetRAMBase(SYSVIEW_RAM_BASE);
}