#define _GNU_SOURCE
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#include <sys/syscall.h>

#define size 20

void MultiplyMatrix();
void printSchedulerName(int PID);

int MatrixA[size][size]={0};
int MatrixB[size][size]={0};
int MatrixC[size][size]={0};
 
pid_t getpid();


int main(int argc, char** argv)
{
	srand(time(NULL));
	int i,j,h, n=100000;
	int mask;
	float a[100000], b[100000], sum;
	time_t second, second2;
	int set_num_threads = 6;
	cpu_set_t set;
	int ret,PID, cpu, prio;
	int cpu_alloc[set_num_threads];
	int migration_counter=0;
	struct sched_param sp;
		
	sp.sched_priority = sched_get_priority_max(SCHED_FIFO);

	
	omp_set_num_threads(set_num_threads);

	#pragma omp parallel num_threads(set_num_threads) private(PID, ret, set, mask)	
	{	
		PID=getpid()+omp_get_thread_num();
		CPU_ZERO(&set);			
		CPU_SET((omp_get_thread_num()%(set_num_threads/2))+1, &set);
		
		ret=sched_setaffinity(PID, sizeof(cpu_set_t), &set);
		sched_setscheduler(PID, SCHED_FIFO, &sp);
		cpu_alloc[omp_get_thread_num()] = sched_getcpu();
		sched_getaffinity(PID, sizeof(mask), (cpu_set_t *)&mask);
		printf("thread%d cpumask%d\n", (int)omp_get_thread_num(), mask);
	}

	second2 = time(NULL);	

	/* Some initializations */

	for (i=0; i < n; i++) {
		a[i] = i*1.0;
		b[n-1-i] = i*1.0;
	}
	
	sum = 0.0;
	for (i=0; i < n; i++)
	{
		sum = sum + (a[i] * b[i]);
		if(sched_getcpu()!=cpu_alloc[omp_get_thread_num()])
		{
			printf("[%d] The thread %d is moved from CPU%d to CPU%d \n", i, omp_get_thread_num(), cpu_alloc[omp_get_thread_num()], sched_getcpu());
			cpu_alloc[omp_get_thread_num()]= sched_getcpu();
			migration_counter++;
		}
	}
	
	//printf("\n Sum = %f\n",sum);

	second = time(NULL);	
	//printf("\n total seconds : %ld\n",second-second2);
	

	for(j=0;j<size;j++)
	{
		for(i=0;i<size;i++)
		{
			MatrixA[i][j] = i*j;
			MatrixB[i][j] = i+j;
			
			if(sched_getcpu()!=cpu_alloc[omp_get_thread_num()])
			{
				printf("The thread %d is moved from CPU%d to CPU%d \n", omp_get_thread_num(), cpu_alloc[omp_get_thread_num()], sched_getcpu());
				cpu_alloc[omp_get_thread_num()]= sched_getcpu();
				migration_counter++;	
			}
		}
	}
/*
	printf("******************************************************\n");
	printf("Matrix A:\n");
	for (i=0; i<size; i++)
	{
	  for (j=0; j<size; j++) [i][j]);
	  printf("\n"); 
	 }
	printf("******************************************************\n");
	printf("Matrix B:\n");
	for (i=0; i<size; i++)
	{
	  for (j=0; j<size; j++) 
	    printf("%d \t", MatrixB[i][j]);
	  printf("\n"); 
	 }
	printf("******************************************************\n");
*/
	second2 = time(NULL);
	
	migration_counter = 0;

	#pragma omp parallel for num_threads(set_num_threads) private(j)
	for (i=0; i < 3000; i++)
	{
		for (j=0; j<3000; j++) 
		{
			MultiplyMatrix();
			if(sched_getcpu()!=cpu_alloc[omp_get_thread_num()])
			{
				printf("The thread %d is moved from CPU%d to CPU%d \n", omp_get_thread_num(), cpu_alloc[omp_get_thread_num()], sched_getcpu());
				cpu_alloc[omp_get_thread_num()]= sched_getcpu();
				migration_counter++;				
			}
		}
	}
	
	/*** Print results ***/
/*
	printf("******************************************************\n");
	printf("Result Matrix:\n");
	for (i=0; i<size; i++)
	{
	  for (j=0; j<size; j++) 
	    printf("%d \t", MatrixC[i][j]);
	  printf("\n"); 
	 }
	printf("******************************************************\n");
	printf ("Done.\n");
*/
	printf("Total number of migration is %d \n", migration_counter);

	second = time(NULL);	
	printf("\n total seconds : %ld\n",second-second2);

}

void MultiplyMatrix()
{
	int i=0, j=0, k=0;
	
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			MatrixC[i][j]=0;
			
			for(k=0;k<size;k++)
			{
				MatrixC[i][j] = MatrixC[i][j]+MatrixB[i][j]* MatrixA[k][j];
				//printf("%d  %d   %d ", MatrixC[i][j],MatrixB[i][j],MatrixA[k][j]);
			}		
		}
	}
}

void printSchedulerName(int PID){	
	int policy = sched_getscheduler(PID);	
	switch(policy){		
	  case SCHED_OTHER:
		printf("Thread: %d, Policy: Normal\n", PID);
		break;
	  case SCHED_RR:
		printf("Thread: %d, Policy: Round-Robin\n", PID);
		break;
	  case SCHED_FIFO:
		printf("Thread: %d, Policy: FIFO\n", PID);
		break;	
	  case -1:
		perror("sched_getscheduler");
		break;
	  default:
		printf("Thread: %d, Policy: Unknown\n", PID);
		break;	
	}
}
