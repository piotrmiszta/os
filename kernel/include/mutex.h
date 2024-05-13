#ifndef KERNEL_MUTEX_H_
#define KERNEL_MUTEX_H_
#include "task.h"
#include "list.h"

struct mutex
{
    struct task* owner;
    struct list tasks;
};

i32 mutex_init(struct mutex* mtx);
i32 mutex_lock(struct mutex* mtx);
i32 mutex_unlock(struct mutex* mtx);
i32 mutex_try_lock(struct mutex* mtx);
i32 mutex_destroy(struct mutex* mtx);

#endif
