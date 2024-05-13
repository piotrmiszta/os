#include "sem.h"
#include "task.h"
#include "list.h"
void sem_init(struct sem* sem, u32 val)
{
    list_init(&sem->tasks);
    sem->val = val;
}

void sem_up(struct sem* sem)
{
    /* post only one thread blocked by this semaphore */
    if(list_is_empty(&sem->tasks))
    {
        sem->val ++;
    }
    else
    {
        struct list* ptr = list_first(&sem->tasks);
        struct task* next = list_get_ptr(ptr, struct task, list_mtx);
        list_del_entry(ptr);
        next->status = TASK_READY;
        sem->val++;
    }
}

void sem_down(struct sem* sem)
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
            struct task* curr = task_get_current();
            curr->status = TASK_LOCK;
            if(!list_is(&curr->list_mtx, &sem->tasks))
                list_add_tail(&curr->list_mtx, &sem->tasks);
            critical_end();
            reschedule();
        }
    }while(1);

}

void sem_destroy(struct sem* sem)
{
    while(!list_is_empty(&sem->tasks))
    {
        struct list* ptr = list_first(&sem->tasks);
        struct task* next = list_get_ptr(ptr, struct task, list_mtx);
        list_del_entry(ptr);
        next->status = TASK_READY;
    }
    list_deinit(&sem->tasks);
    sem->val = 0;
}