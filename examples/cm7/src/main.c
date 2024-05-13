#include "stm32h7xx.h"
#include "os_task.h"
#include "types.h"
#include "uart.h"
#include "os_mutex.h"
#include "list.h"
#include "os_sem.h"

struct helper
{
    struct list list;
    i32 val;
};

i32 task1(void* param);
i32 task2(void* param);

static struct os_mutex mtx;
static struct helper tab[100];
struct list que = {&que, &que};
static struct os_sem sem_full;
static struct os_sem sem_empty;

int main(void)
{
    struct os_task t1;
    struct os_task t2;
    usart3_init(115200);
    RCC->AHB4ENR |= (1U << 4);
    GPIOE->MODER |= (1U << 2);
    GPIOE->MODER &= ~(1U << 3);
    os_sem_init(&sem_full, 0);
    os_sem_init(&sem_empty, 10);
    os_mutex_init(&mtx);
    LOG_DEBUG("Init\n");
    u32 stack1[12800];
    u32 stack2[12800];


    os_task_init(&t1, task1, NULL, 1, stack1, sizeof(stack1));
    os_task_init(&t2, task2, NULL, 2, stack2, sizeof(stack2));
    os_sched_start(9000);
    assert(0);
}

int writed = 0;

i32 task1(void* param)
{
    (void)param;
    while(1)
    {
       // for(int i = 0; i < 10000000; i++) {}
        os_sem_down(&sem_empty);
        struct helper* ptr = &tab[writed%100];
        ptr->val = writed;
        writed++;
        list_add_tail(&ptr->list, &que);
        os_sem_up(&sem_full);
    }
}

i32 task2(void* param)
{
    (void)param;
    while(1)
    {
        os_sem_down(&sem_full);
        struct list* ptr = list_first(&que);
        list_del_entry(ptr);
        struct helper* entry = list_get_ptr(ptr, struct helper, list);
        LOG_DEBUG("Get %d from que\n", entry->val);
        os_sem_up(&sem_empty);
    }
}