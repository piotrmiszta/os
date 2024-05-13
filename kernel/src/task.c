#include "stm32h7xx.h"
#include "os_task.h"
#include "list.h"

//TODO: move to arch
#define SYSTICK_ENABLE      (1U << 0)
#define SYSTICK_TICKINT     (1U << 1)
#define SYSTICK_CLKSOURCE   (1U << 2)
const u32 no_regs = 16;
const u32 systick_prio = 0x0U;
const u32 pendsv_prio = 0xffU;

/* global variables */
LIST_CREATE(task_list);
static volatile struct os_task* active_task;
static volatile struct os_task* next_task;

/* static functions */
static void os_sched_get_next_task(void);
static i32 os_task_end(void* null);

struct os_task* os_task_get_current(void)
{
    return (struct os_task*)active_task;
}

void os_reschedule(void)
{
    os_sched_get_next_task();
    SCB->ICSR |= (1U << 28);
}

i32 os_task_init(  struct os_task* task,
                i32 (*handler)(void*),
                void* params,
                u32 prio,
                u32* sp,
                u32 stack_size)
{
    u32 offset = (u32)(stack_size / sizeof(u32));
    task->handler = handler;
    task->params = params;
    task->prio = prio;
    task->sp = (u32)(sp + offset - no_regs);
    task->status = TASK_READY;
    task->id = list_size(&task_list);
    /* init stack of new thread */
    sp[offset - 1] = 0x01000000;                //psp register
    sp[offset - 2] = (u32)handler &~ 0x01UL;    //pc register
    sp[offset - 3] = (u32)os_task_end;             //lr register
    sp[offset - 8] = (u32)params;               //r0 register - arg

    /* push task to list */
    list_add_tail(&(task->list), &task_list);
    return 0;
}

void os_sched_start(u32 ticks)
{
    /* set systick and prio of systick pendsv*/
    /*TODO: Move architecture depend code into new arch/ directory */
    #ifdef CORE_CM7
    SCB->SHPR[2] |= (u8)(pendsv_prio << 16);
    SCB->SHPR[2] |= (u8)(systick_prio << 24);
    #elif CORE_CM4
    SCB->SHP[2] |= (u8)(pendsv_prio << 16);
    SCB->SHP[2] |= (u8)(systick_prio << 24);
    #endif
    SysTick->LOAD = ticks - 1;
    SysTick->CTRL |= SYSTICK_ENABLE;
    SysTick->CTRL |= SYSTICK_TICKINT;
    SysTick->CTRL |= SYSTICK_CLKSOURCE;
    /* get first task to execute */
    list_t* first_task = list_first(&task_list);
    active_task = list_get_task(first_task);
    /* set cpu registers */
    u32 stack = active_task->sp + (no_regs * sizeof(u32));
    u32 control = 0x02;
    ASM("MSR PSP, %0" : : "r"(stack));
    ASM("MSR control, %0" : : "r" (control) : "memory");
    ASM("ISB 0xF" : : : "memory");
    /* call function */
    active_task->handler(active_task->params);
}

static void os_sched_get_next_task(void)
{
    active_task->status = TASK_READY;
    list_t* ptr = (list_t*)(&active_task->list);
    while(1) {
        ptr = list_next(ptr);
        if(list_is_head(ptr, &task_list)) {
            continue;
        }
        next_task = list_get_task(ptr);
        if(next_task->status == TASK_READY) {
            break;
        }
        else if(next_task->status == TASK_LOCK) {
            continue;
        }
    }
    next_task->status = TASK_ACTIVE;
}

ATTRIBUTE(noreturn)
static i32 os_task_end(void* null)
{
    (void)null;
    while(1) {}
}

void SysTick_Handler(void) {
    critical_start();
    os_sched_get_next_task();
    SysTick->VAL = 0;
    critical_end();
    SCB->ICSR |= (1U << 28);
}

ATTRIBUTE( isr, naked )
void PendSV_Handler(void)
{
    __asm volatile(
    "mrs    r0, psp \n"
    "subs   r0, #16 \n"
    "stmia  r0!,{r4-r7} \n"
    "mov    r4, r8 \n"
    "mov    r5, r9 \n"
    "mov    r6, r10 \n"
    "mov    r7, r11 \n"
    "subs   r0, #32 \n"
    "stmia  r0!,{r4-r7} \n"
    "subs   r0, #16 \n"             /* Load next task's SP: */
    "ldr    r2, =active_task \n"
    "ldr    r1, [r2] \n"
    "str    r0, [r1] \n"
    "ldr    r2, =next_task \n"
    "ldr    r1, [r2] \n"
    "ldr    r0, [r1] \n"
    "ldmia  r0!,{r4-r7} \n"
    "mov    r8, r4 \n"
    "mov    r9, r5 \n"
    "mov    r10, r6 \n"
    "mov    r11, r7 \n"
    "ldmia  r0!,{r4-r7} \n"
    "msr    psp, r0 \n"
    "ldr r0, =0xFFFFFFFD \n"
    "ldr    r1, =active_task \n"
    "ldr    r2, =next_task \n"
    "ldr    r3, [r2] \n"
    "str    r3, [r1] \n"
    "bx r0 \n"
    );
}