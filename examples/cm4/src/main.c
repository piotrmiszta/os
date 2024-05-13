#include "stm32h7xx.h"
#include "os_task.h"

i32 task1(void* param);
i32 task2(void* param);

/*
 * Simple test of context switch and scheduler algorithm
 * For now scheduler is non priority based, only RR algorithm is used
 * Scenario:
 *  task1 - turn builtin led on
 *  task2 - turn builtin led off
 */

int main(void)
{
    RCC->AHB4ENR |= (1U << 1);
    GPIOB->MODER |= (1U << 28);
    GPIOB->MODER &= ~(1U << 29);
    u32 stack1[12800];
    u32 stack2[12800];
    struct os_task t1;
    struct os_task t2;
    os_task_init(&t1, task1, NULL, 1, stack1, sizeof(stack1));
    os_task_init(&t2, task2, NULL, 2, stack2, sizeof(stack2));
    os_sched_start(9000000);
    assert(0);
}

i32 task1(void* param)
{
    (void)param;
    while(1)
    {
        GPIOB->ODR |= (1U << 14);
    }
}

i32 task2(void* param)
{
    (void)param;
    while(1)
    {
        GPIOB->ODR &= ~(1U << 14);
    }
}