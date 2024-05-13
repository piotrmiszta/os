#ifndef KERNEL_SEM_H_
#define KERNEL_SEM_H_
#include "task.h"
#include "types.h"
#include "list.h"

struct sem
{
    u32 val;
    u32 mval;
    struct list tasks;
};

void sem_init(struct sem* sem, u32 val);
void sem_down(struct sem* sem);
void sem_up(struct sem* sem);
void sem_destroy(struct sem* sem);

#endif
