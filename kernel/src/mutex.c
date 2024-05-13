#include "mutex.h"

i32 mutex_init(struct mutex* mtx)
{
    mtx->owner = NULL;
    list_init(&mtx->tasks);
    return 0;
}

i32 mutex_lock(struct mutex* mtx)
{
    struct task* curr = task_get_current();
    do
    {
        critical_start();
        if(mtx->owner == NULL)
        {
            mtx->owner = curr;
            break;
        }
        else if(mtx->owner == curr)
        {
            break;
        }
        else
        {
            curr->status = TASK_LOCK;
            if(!list_is(&curr->list_mtx, &mtx->tasks))
            {
                list_add_tail(&curr->list_mtx, &mtx->tasks);
            }
            reschedule();
        }
        critical_end();
    } while (1);
    critical_end();
    return 0;
}

i32 mutex_unlock(struct mutex* mtx)
{
    critical_start();
    if(mtx->owner != task_get_current())
    {
        critical_end();
        return -1;
    }
    mtx->owner = NULL;
    while(!list_is_empty(&mtx->tasks))
    {
        struct list* lock = list_first(&mtx->tasks);
        list_del_entry(lock);
        struct task* next = list_get_ptr(lock, struct task, list_mtx);
        next->status = TASK_READY;
    }
    critical_end();
    return 0;
}

i32 mutex_try_lock(struct mutex* mtx)
{
    critical_start();
    if(mtx->owner == NULL)
    {
        mtx->owner = task_get_current();
        critical_end();
        return 0;
    }
    else
    {
        critical_end();
        return -1;
    }
}

i32 mutex_destroy(struct mutex* mtx)
{
    if(mutex_unlock(mtx))
    {
        return -1;
    }
    list_init(&mtx->tasks);
    return 0;
}
