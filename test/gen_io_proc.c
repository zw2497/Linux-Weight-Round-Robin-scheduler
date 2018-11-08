#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CPUS 6 /* We will be testing only on the VMs */
#define __NR_get_wrr_info 326
#define __NR_set_wrr_weight 327
#define __NR_setscheduler 144
#define CHILD_PROC 100

struct wrr_info {
	int num_cpus;
	int nr_running[MAX_CPUS];
	int total_weight[MAX_CPUS];
};
struct sched_param {
	int sched_priority;
};

int main(int argc, char **argv)
{
	struct wrr_info info;
	struct sched_param param;
	pid_t pid;
	int res;
	int number = 0;
	int n = 0;
	int status;
	int c = 0;
	

	pid = getpid();
	param.sched_priority = 0;
	syscall(__NR_setscheduler, pid, 7, &param);
	syscall(__NR_set_wrr_weight, 1);
	
	res = syscall(__NR_get_wrr_info, &info);
        if (!res) {
                printf("num_cpus: %d\n", info.num_cpus);
                for (int i = 0; i < info.num_cpus; i++) {
                        printf("CPU_%d: %d %d\n", i, info.nr_running[i],
                                                info.total_weight[i]);
                }
        }
		
	


	for (int i = 0; i < CHILD_PROC; i++) {
		pid = fork();
		if (pid < 0)
			return -1;

		if (pid == 0) {
			syscall(__NR_setscheduler, getpid(), 7, &param);
			while (1) {
				scanf("%d", &c);
				printf("The number you imput is %d\n", c);
				if (n == 500000000){
					printf("Weight: 50, pid: %d, run at:%d\n", getpid(), number);
					number++;
					n = 0;
					
					if (number % 5 == 0) {
						res = syscall(__NR_get_wrr_info, &info);
						if (!res) {
							printf("num_cpus: %d\n", info.num_cpus);
							for (int i = 0; i < info.num_cpus; i++) {
								printf("CPU_%d: %d %d\n", i, info.nr_running[i],
											info.total_weight[i]);
							}
						}
					}
				}
				
				
				
				if (number == 30) {
					printf("\nI am exited\n");
					
					exit(0);
				}
				n++;
			}
		}
	}
	for (int i = 0; i < CHILD_PROC; i++)
		wait(&status);
	

	return 0;
}
