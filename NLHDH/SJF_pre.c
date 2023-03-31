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
    response_time = waiting_time;
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

int main(){
    
    int n;
    printf("Nhap so luong tien trinh:\n");
    scanf("%d",&n);
	struct process P[n-1] ;

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
	SJF_preemptive(P, n);
   
    return 0;
}
