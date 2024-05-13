#include "os_sem.h"
#include "os_task.h"
#include "list.h"
void os_sem_init(struct os_sem* sem, u32 val)
{
    list_init(&sem->tasks);
    sem->val = val;
}

void os_sem_up(struct os_sem* sem)
{
    /* post only one thread blocked by this semaphore */
    if(list_is_empty(&sem->tasks))
    {
        sem->val ++;
    }
    else
    {
        struct list* ptr = list_first(&sem->tasks);
        struct os_task* next = list_get_ptr(ptr, struct os_task, list_mtx);
        list_del_entry(ptr);
        next->status = TASK_READY;
        sem->val++;
    }
}

void os_sem_down(struct os_sem* sem)
{
    do
    {
        if(sem->val > 0)
        {
            sem->val--;
            break;
        }
        else
        {
            struct os_task* curr = os_task_get_current();
            curr->status = TASK_LOCK;
            if(!list_is(&curr->list_mtx, &sem->tasks))
                list_add_tail(&curr->list_mtx, &sem->tasks);
            critical_end();
            os_reschedule();
        }
    }while(1);

}

void os_sem_destroy(struct os_sem* sem)
{
    while(!list_is_empty(&sem->tasks))
    {
        struct list* ptr = list_first(&sem->tasks);
        struct os_task* next = list_get_ptr(ptr, struct os_task, list_mtx);
        list_del_entry(ptr);
        next->status = TASK_READY;
    }
    list_deinit(&sem->tasks);
    sem->val = 0;
}