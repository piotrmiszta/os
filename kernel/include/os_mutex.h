#ifndef KERNEL_MUTEX_H_
#define KERNEL_MUTEX_H_
#include "os_task.h"
#include "list.h"

struct os_mutex
{
    struct os_task* owner;
    struct list tasks;
};

i32 os_mutex_init(struct os_mutex* mtx);
i32 os_mutex_lock(struct os_mutex* mtx);
i32 os_mutex_unlock(struct os_mutex* mtx);
i32 os_mutex_try_lock(struct os_mutex* mtx);
i32 os_mutex_destroy(struct os_mutex* mtx);

#endif
