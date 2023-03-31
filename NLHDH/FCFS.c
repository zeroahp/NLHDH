#include <stdio.h>
#define Min -9999999

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

    int max = Min;
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

int main(){
    
    int n;
    printf("Nhap so luong tien trinh:\n");
    scanf("%d",&n);
    struct process P[n-1];
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
    fcfs(P,n);
    
    return 0;
}
