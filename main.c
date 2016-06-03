#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "thread_pool.h"

void * myprocess (void *arg) 
{ 
    printf ("threadid is 0x%x, working on task %d\n", pthread_self (),*(int *) arg); 
    sleep (1);/*休息一秒，延长任务的执行时间*/ 
    return NULL; 
} 

int main (int argc, char **argv) 
{ 
    int res;
    
    res = pool_init (3);/*线程池中最多三个活动线程*/
    if(res != 3)
    {
        printf("pool_init() failed!\n");
        exit(1);
    } 
     
    /*连续向池中投入10个任务*/ 
    int *workingnum = (int *) malloc (sizeof (int) * 10); 
    if(workingnum == NULL)
    {
        perror("malloc() workingnum fialed");
        pool_destroy (); 
        exit(1);
    }
    int i; 
    for (i = 0; i < 10; i++) 
    { 
        workingnum[i] = i; 
        res = pool_add_task (myprocess, &workingnum[i]); 
        if(res)
        {
            printf("pool_add_task() failed!\n");
            exit(1);
        }
    } 
    /*等待所有任务完成*/ 
    sleep (5); 
    /*销毁线程池*/ 
    res = pool_destroy (); 
    if(res)
    {
        printf("pool_destroy() failed!\n");
        exit(1);
    }

    free (workingnum); 
    return 0; 
}

