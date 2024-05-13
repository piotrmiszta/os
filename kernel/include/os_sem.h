#ifndef KERNEL_SEM_H_
#define KERNEL_SEM_H_
#include "os_task.h"
#include "types.h"
#include "list.h"

struct os_sem
{
    u32 val;
    u32 mval;
    struct list tasks;
};

void os_sem_init(struct os_sem* sem, u32 val);
void os_sem_down(struct os_sem* sem);
void os_sem_up(struct os_sem* sem);
void os_sem_destroy(struct os_sem* sem);

#endif
