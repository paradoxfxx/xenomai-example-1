#include <iostream>
#include <native/task.h>
#include <native/timer.h>
#include <string>
#include <fstream>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define PERIOD 100000 //RT tasks period(ns)
#define NUM 30 // number of RT tasks
#define DNUM 10000 // number of threads cycle

RT_TASK measureTask;
RT_TASK task[NUM];
std::ofstream file("./data.txt");
RTIME *data = new RTIME[DNUM];

void measureThread(void* arg){
	int cnt = 0;
	rt_task_set_periodic(NULL, TM_NOW, PERIOD);
	while(cnt < DNUM){
		rt_task_wait_period(NULL);
		data[cnt] = rt_timer_read();
		++cnt;
	}

	for(int i=0; i<NUM; ++i){
		rt_task_delete(&task[i]);
	}
	for(int i=0; i<DNUM; ++i){
		file << data[i] << std::endl;
	}

	delete[] data;
	std::cout<< "End of Test." << std::endl;
	exit(1);
}

void dummyThread(void* arg){
	int cnt = 0;
	rt_task_set_periodic(NULL, TM_NOW, PERIOD);
	while(1){
		rt_task_wait_period(NULL);
		++cnt;
	}
}

int main(int argc, char* argv[]){
	char RTName[] = "taskN";
	mlockall(MCL_CURRENT | MCL_FUTURE);
	
	rt_task_create(&measureTask, "Measure Task", 0, 99, 0);
	std::cout<< "create Measurement RT task" << std::endl;
	
	for(int i=0; i<NUM; ++i){
		RTName[4] = '0'+i;
		rt_task_create(&task[i], RTName, 0, 99, 0);
		std::cout<< "create RT task: " << RTName << std::endl;

	}
	for(int i=0; i<NUM; ++i){
		rt_task_start(&task[i], &dummyThread, (void*)NULL);
	}
	rt_task_start(&measureTask, &measureThread, (void*)NULL);

	pause();

	return 0;
}
