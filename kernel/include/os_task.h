#ifndef TASK_H_
#define TASK_H_
#include "list.h"
#include "types.h"

#define list_get_task(ptr)                  \
        list_get_ptr(ptr, struct os_task, list)


/**
 * @enum TaskStateE
 * @brief   Status of task
 *
 */
enum TaskStateE
{
    TASK_ACTIVE = 0,/**< TASK_ACTIVE - task is executing*/
    TASK_READY = 1, /**< TASK_READY - task is ready to be selected by scheduler*/
    TASK_LOCK = 2,  /**< TASK_LOCK - task is lock by mutex or semaphore */
    TASK_END = 3,   /**< TASK_END - task is end, no longer executable */
};

/**
 * @struct task
 * @brief   task structure that describe all information
 *          that are needed for execution task
 *
 */
struct os_task
{
    u32 sp;                                 //stack pointer
    u32 prio;                               //priority
    void* params;                           //params to call function
    i32 (*handler)(void*);                  //function to call at start of thread
    i32 id;
    volatile enum TaskStateE status;        //status of thread
    struct list list;                       //list handler
    struct list list_mtx;
};

/**
 * @brief task_init initialize a new task
 *
 * @param task pointer to task structure
 * @param handler pointer to function that should be executed by thread
 * @param params parameters that should be passed to handler
 * @param prio priority of task
 * @param sp pointer to stack of task
 * @param stack_size size of stack
 * @return 0 on success
 */
i32 os_task_init(  struct os_task* task,
                i32 (*handler)(void*),
                void* params,
                u32 prio,
                u32* sp,
                u32 stack_size);

/**
 * @fn void sched_start(u32)
 * @brief   start scheduler, enable systick and call first task's handler
 *          function noreturn
 * @param ticks systick ticks
 *
 */
void os_sched_start(u32 ticks);

/**
 * @fn struct task task_get_current*(void)
 * @brief return pointer to struct task of currently executed task
 *
 * @return
 */
struct os_task* os_task_get_current(void);

/**
 * @fn void reschedule(void)
 * @brief force systick
 *
 * @pre
 * @post
 */
void os_reschedule(void);
#endif /* TASK_H_ */