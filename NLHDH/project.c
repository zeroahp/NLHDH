#include <stdio.h>
#define MIN -9999999
#define MAX 9999999
struct process {
    int id; //id
    int arrival_time;   // thời gian đến hệ thống
    int CPU_time;     // thời gian chiếm dụng CPU
    int waiting_time;   // thời gian chờ đợi
    int response_time;  // thời gian phản hồi
    int turnaround_time;// thời gian hoàn thành
    int complete_time;
};

void print_gantt_chart(struct process *p, char chart[], int max, int num_process) {
    int id = 0,i,j;

    while(id < num_process) {
        for (j = 0; j < p[id].arrival_time; j++)
            chart[j] = '-';
        
        int tmp = p[id].waiting_time;
        int cnt = p[id].arrival_time;
        
        while (tmp--)
            chart[cnt++] = 'W';
        
        tmp = p[id].CPU_time;
        while (tmp--)
            chart[cnt++] = 'R';

        while (cnt < max)
            chart[cnt++] = '-';

        for(j = 0; j < max ; j++){
            printf("%c", chart[j]);
            chart[j] = '\0';
        }
        printf("\n");
        id++;
    } 
}

void fcfs(struct process *processes, int n) {
    int current_time = 0;
    int waiting_time = 0;
    int turnaround_time = 0;
    int response_time = 0;
    int i,j;
    for (int i = 0; i < n-1; i++) {
        for(j = 0; j < n-i-1 ; j++){
            if(processes[j].arrival_time > processes[j+1].arrival_time)
            {
                float tmp;

                tmp = processes[j].id;
				processes[j].id = processes[j+1].id;
				processes[j+1].id = tmp;

                tmp = processes[j].CPU_time;
				processes[j].CPU_time = processes[j+1].CPU_time;
				processes[j+1].CPU_time = tmp;
			
				tmp = processes[j].arrival_time;
				processes[j].arrival_time = processes[j+1].arrival_time;
				processes[j+1].arrival_time = tmp;
				
            }
        }
    }

    int max = MIN;
    float response_t = 0, turnaround_t = 0 , waiting_t = 0;
    int tong = 0;
    for(i = 0; i < n; i++){
        tong += processes[i].CPU_time;
        processes[i].complete_time = tong;
        processes[i].turnaround_time = processes[i].complete_time - processes[i].arrival_time;
        processes[i].response_time =  processes[i].turnaround_time;
        
        if(max < processes[i].turnaround_time + processes[i].arrival_time)
            max = processes[i].turnaround_time + processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].CPU_time;

        turnaround_t = turnaround_t + processes[i].turnaround_time;
        waiting_t = waiting_t + processes[i].waiting_time;
        
        if(tong < processes[i+1].arrival_time){
            float time = processes[i+1].arrival_time - tong;
            tong += time;
        }
    }

    // tính giá trị trung bình của waiting time, response time và turnaround time
    waiting_t = waiting_t / n;
    response_t = waiting_t;
    turnaround_t = turnaround_t / n;

    // in ra kết quả
    printf("\nDinh thoi FCFS:\n\n");
    char chart[max-1];
    print_gantt_chart(processes,chart, max, n);
    printf("\n");
    printf("AVGT=%.2f\t",turnaround_t);
    printf("AVGR=%.2f\t",response_t);
    printf("AVGW=%.2f\t",waiting_t);
}

/**********************************SJF_preemptive****************************************/
void SJF_preemptive(struct process *process, int n)
{
    int i;
	for (i = 0; i < n; i++)
        process[i].response_time = process[i].CPU_time; 

	int complete = 0, t = 0, minm = MAX;
	int shortest = 0, finish_time;
	bool check = false;

	int j;
	while (complete != n) {
		for (j = 0; j < n; j++) {
			
            if ((process[j].arrival_time <= t) && (process[j].response_time < minm) && process[j].response_time > 0) {
				minm = process[j].response_time;
				shortest = j;
				check = true;
			}
		}
		if (check == false) {
			t++;
			continue;
		}
		process[shortest].response_time--;

		minm = process[shortest].response_time;
		
		if (minm == 0) minm = MAX;

		if (process[shortest].response_time == 0) {

			complete++;
			check = false;
			finish_time = t + 1;

			process[shortest].waiting_time = finish_time - process[shortest].CPU_time - process[shortest].arrival_time;

			if (process[shortest].waiting_time < 0)
				process[shortest].waiting_time = 0;
		}
		t++;
	}
	

	for (i = 0; i < n; i++)
		process[i].turnaround_time = process[i].CPU_time + process[i].waiting_time;
	
    float turnaround_t = 0;
    float response_time = 0;
	float waiting_time = 0;
	int max_time = MIN;
	
	for (i = 0; i < n; i++)
	{
		turnaround_t += process[i].turnaround_time;
		if (max_time < process[i].turnaround_time + process[i].arrival_time) 
			max_time = (process[i].turnaround_time + process[i].arrival_time);
		waiting_time += process[i].waiting_time;
        
	}
	
	waiting_time = waiting_time / n;
    response_time = waiting_time ;
    turnaround_t = turnaround_t / n;

    // in ra kết quả
    printf("\nDinh thoi SJF pre_emptive\n\n");
    char chart[max_time-1];

    print_gantt_chart(process,chart, max_time, n);
    printf("\n");
    printf("AVGT=%.2f\t",turnaround_t);
    printf("AVGR=%.2f\t",response_time);
    printf("AVGW=%.2f\t",waiting_time);

}

/**********************************ROUND_ROBIN****************************************/
void queue(int queue[], int m)
{
    int i;
    for (i = 0; i < m - 1 && queue[i + 1] != 0; i++)
    {
        int temp = queue[i];
        queue[i] = queue[i + 1];
        queue[i + 1] = temp;
    }
}

void queueNew(struct process *p, int queue[], int size, int max)
{
    int j, i;
    for (i = 0; i < size; i++)
    {
        if (queue[i] == 0)
        {
            j = i;
            break;
        }
    }
    queue[j] = max + 1;
}

void kiemtraarrivaltime(struct process *p, int time, int n, int max, int queue[])
{
    int i, j;
    if (time <= p[n - 1].arrival_time)
    {
        bool newArrival = false;
        for (i = max + 1; i < n; i++)
        {
            if (p[i].arrival_time <= time)
            {
                if (max < i)
                {
                    max = i;
                    newArrival = true;
                }
            }
        }
        if (newArrival)
            queueNew(p, queue, n, max);
    }
}
void Round_Robin(struct process *p, int n, int q)
{
    int time = 0, max = 0, i;
    int queue_t[n], cpu_t[n];
    bool complete[n];
    for (i = 0; i < n; i++)
    {
        complete[i] = false;
        queue_t[i] = 0;
        cpu_t[i] = p[i].CPU_time;
    }
    while (time < p[0].arrival_time)
        time++;
    queue_t[0] = 1;
    while (true)
    {
        bool flag = true;
        for (i = 0; i < n; i++)
        {
            if (cpu_t[i] != 0)
            {
                flag = false;
                break;
            }
        }
        if (flag)
            break;
        for (i = 0; i < n && queue_t[i] != 0; i++)
        {
            int ctr = 0;
            while (ctr < q && cpu_t[queue_t[0] - 1] > 0)
            {
                cpu_t[queue_t[0] - 1] -= 1;
                time += 1;
                ctr++;
                kiemtraarrivaltime(p, time, n, max, queue_t);
            }
            if (!cpu_t[queue_t[0] - 1] && complete[queue_t[0] - 1] == false)
            {
                p[queue_t[0] - 1].turnaround_time = time;
                complete[queue_t[0] - 1] = true;
            }
            bool id = true;
            if (!queue_t[n - 1])
            {
                for (i = 0; i < n && queue_t[i] != 0; i++)
                {
                    if (complete[queue_t[i] - 1] == false)
                        id = false;
                }
            }
            else
                id = false;
            if (id)
            {
                time++;
                kiemtraarrivaltime(p, time, n, max, queue_t);
            }
            queue(queue_t, n);
        }
    }
    for (i = 0; i < n; i++)
    {
        p[i].turnaround_time = p[i].turnaround_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].CPU_time;
    }

    float turnaround_t = 0;
    float response_time = 0;
	float waiting_time = 0;
	int max_time = MIN;
	
	for (i = 0; i < n; i++)
	{
		turnaround_t += p[i].turnaround_time;
		if (max_time < p[i].turnaround_time + p[i].arrival_time) 
			max_time = (p[i].turnaround_time + p[i].arrival_time);
		waiting_time += p[i].waiting_time;
	}

    turnaround_t = turnaround_t / n;
	waiting_time = waiting_time / n;
    response_time = turnaround_t;

    // in ra kết quả
    printf("\nDinh thoi Round Robin\n\n");
    char chart[max_time-1];

    print_gantt_chart(p,chart, max_time, n);
    printf("\n");
    printf("AVGT=%.2f\t",turnaround_t);
    printf("AVGR=%.2f\t",response_time);
    printf("AVGW=%.2f\t",waiting_time);

}

int main(){
    
    int n;
    printf("Nhap so luong tien trinh:\n");
    scanf("%d",&n);
	struct process P[n-1] ;
    float q;
    printf("Nhap quantum: \n");
    scanf("%f", &q);
	int i;
    for(i=0 ; i < n ;i++){
        printf("Tien trinh thu %d :\n",(i+1));
        printf("Nhap Arrival time:");
        scanf("%d",&P[i].arrival_time);
        printf("Nhap CPU time: ");
        scanf("%d",&P[i].CPU_time);
        P[i].id = i + 1;
        printf("\n");

    }
    // fcfs(P,n);
    // printf("\n\n******************************************\n");
    // SJF_preemptive(P,n);
    printf("\n\n******************************************\n");
    Round_Robin(P,n,q);
    
    return 0;
}
