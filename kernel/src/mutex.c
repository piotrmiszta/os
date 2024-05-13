#include "os_mutex.h"

i32 os_mutex_init(struct os_mutex* mtx)
{
    mtx->owner = NULL;
    list_init(&mtx->tasks);
    return 0;
}

i32 mutex_lock(struct os_mutex* mtx)
{
    struct os_task* curr = os_task_get_current();
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
            os_reschedule();
        }
        critical_end();
    } while (1);
    critical_end();
    return 0;
}

i32 os_mutex_unlock(struct os_mutex* mtx)
{
    critical_start();
    if(mtx->owner != os_task_get_current())
    {
        critical_end();
        return -1;
    }
    mtx->owner = NULL;
    while(!list_is_empty(&mtx->tasks))
    {
        struct list* lock = list_first(&mtx->tasks);
        list_del_entry(lock);
        struct os_task* next = list_get_ptr(lock, struct os_task, list_mtx);
        next->status = TASK_READY;
    }
    critical_end();
    return 0;
}

i32 os_mutex_try_lock(struct os_mutex* mtx)
{
    critical_start();
    if(mtx->owner == NULL)
    {
        mtx->owner = os_task_get_current();
        critical_end();
        return 0;
    }
    else
    {
        critical_end();
        return -1;
    }
}

i32 os_mutex_destroy(struct os_mutex* mtx)
{
    if(os_mutex_unlock(mtx))
    {
        return -1;
    }
    list_init(&mtx->tasks);
    return 0;
}
