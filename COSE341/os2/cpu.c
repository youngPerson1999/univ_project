#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdint.h>
#include <sched.h>
#include <string.h>
#include <sys/syscall.h>
#define ROW (100)
#define COL ROW

int count, t_time, ex_time, p_id;
struct sched_attr {
   uint32_t	size;
   uint32_t	sched_policy;
   uint64_t	sched_flags;
   int32_t	sched_nice;
   uint32_t	sched_priority;
   uint64_t	sched_runtime;
   uint64_t	sched_deadline;
   uint64_t	sched_period;
};

static int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags)
{
   return syscall(SYS_sched_setattr, pid, attr, flags);
}

void proc_end()
{
    printf("DONE!! PROCESS #%02d : %06d %d\n", p_id, count, t_time);
}

void INThandler(int sig)
{
    proc_end();
    exit(0);
}

int calc(int p, int t)
{
    struct timespec start, end, total;
    int matrixA[ROW][COL];
    int matrixB[ROW][COL];
    int matrixC[ROW][COL];
    int i, j, k;
    count = 0;
    p_id = p;

    clock_gettime(CLOCK_MONOTONIC, &start);
    clock_gettime(CLOCK_MONOTONIC, &total);
    while (1)
    {
        for (i = 0; i < ROW; i++)
        {
            for (j = 0; j < COL; j++)
            {
                for (k = 0; k < COL; k++)
                {
                    matrixC[i][j] += matrixA[i][j] * matrixB[i][j];
                }
            }
        }

        count++;
        clock_gettime(CLOCK_MONOTONIC, &end);
        // tv_sec -> 1000ms, tv_nsec -> 1ns = 10^-6ms
        ex_time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000;

        if (ex_time >= 100)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
            printf("PROCESS #%02d count = %d %d\n", p, count, ex_time);
            t_time = 1000 * (end.tv_sec - total.tv_sec) + (end.tv_nsec - total.tv_nsec) / 1000000;
            if (t_time >= t)
            {
                proc_end();
                break;
            }
        }
    }

    return (0);
}

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        return (0);
    }

	struct sched_attr attr;
	memset(&attr, 0, sizeof(attr));
	attr.size = sizeof(struct sched_attr);
	
	attr.sched_priority = 10;
	attr.sched_policy = SCHED_RR;

    int p_num = atoi(argv[1]);
    int t_time = atoi(argv[2]);
    int i;
    pid_t pid = getpid();

	int result = sched_setattr(pid, &attr, 0);
	if(result == -1){
	   printf("Error in sched_setattr\n");
	}

    for (i = 0; i < p_num; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "fork fail\n");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            printf("Creating Process: #%02d\n", i);
        }
        else
        {
            signal(SIGINT, INThandler);
            calc(i, t_time * 1000);
            break;
        }
    }
    if (pid > 0)
        signal(SIGINT, SIG_IGN);

    while (wait(NULL) > 0)
        ;
    return EXIT_SUCCESS;
}
