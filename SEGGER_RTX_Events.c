#include "SEGGER_SYSVIEW.h"
#include "rtx_os.h"
#include <string.h>

void EvrRtxThreadCreated(osThreadId_t thread_id, uint32_t thread_addr, const char *name)
{
    osRtxThread_t *thread = (osRtxThread_t *)thread_id;

    SEGGER_SYSVIEW_TASKINFO info;
    memset(&info, 0, sizeof(info));
    info.TaskID = (unsigned long)thread_id;
    info.sName = thread->name;
    info.Prio = thread->priority;
    info.StackBase = (uint32_t)thread->stack_mem;
    info.StackSize = thread->stack_size;

    SEGGER_SYSVIEW_OnTaskCreate((uint32_t)thread_id);
    SEGGER_SYSVIEW_SendTaskInfo(&info);
}

void EvrRtxThreadUnblocked(osThreadId_t thread_id, uint32_t ret_val)
{
    SEGGER_SYSVIEW_OnTaskStartReady((uint32_t)thread_id);
}

void EvrRtxThreadSwitched(osThreadId_t thread_id)
{
    osRtxThread_t *thread = (osRtxThread_t *)thread_id;

    if (thread->priority == osPriorityIdle)
    {
        SEGGER_SYSVIEW_OnIdle();
    }
    else
    {
        SEGGER_SYSVIEW_OnTaskStartExec((uint32_t)thread_id);
    }
}

void EvrRtxThreadBlocked(osThreadId_t thread_id, uint32_t timeout)
{
    // TODO different events could be used to derive block cause
    SEGGER_SYSVIEW_OnTaskStopReady((uint32_t)thread_id, timeout);
}