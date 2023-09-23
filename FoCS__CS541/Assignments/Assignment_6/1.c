/******************************************************************************
* Executive M.Tech (AI & DSE)
* Subject: CS559 - Computer Systems Lab
* Name: Manjit Singh Duhan
* Addmission No: IITP001316
* Roll No : 2303res134
* email id: duhan.manjit@gmail.com
* Assignement 6
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct node {
    int     process_number;
    int     arrival_time;
    int     burst_time;
    int     CT;
    int     TAT;
    int     WT;
};

struct node *processes = NULL;

/******************************************************************************
* \brief  swap the two nodes passed
*
* \param[in/out] a: Address of node a which of type struct node
* \param[in/out] b: Address of node b which of type struct node
*
* *****************************************************************************/
void swap_nodes(struct node* a, struct node* b)
{
    struct node temp;

    memcpy(&temp, a, sizeof(struct node));
    memcpy(a, b, sizeof(struct node));
    memcpy(b, &temp, sizeof(struct node));
}

/******************************************************************************
* \brief  arrange the processes accoing to the arrival time
*
* \param[in] arr: arrary containg the detail of processes
* \param[in] number_of_processes: total number of process entered by user
*
* *****************************************************************************/
void do_FCFS_scheduling(struct node arr[], int number_of_processes)
{
    bool swapped;

    for (int i = 0; i < number_of_processes - 1; i++) {
        swapped = false;
        
        for (int j = 0; j < number_of_processes - i - 1; j++) {
            
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {

                swap_nodes(&arr[j], &arr[j + 1]);

                swapped = true;
            }
        }

        // If no swap is done then it means the array got sorted and so, we can return
        if (swapped == false) {
            return ;
        }
    }
}

/******************************************************************************
* \brief Print the Gantt chart
*
* \param[in] number_of_processes: total number of process entered by user
*
* *****************************************************************************/
void print_gantt_chart(int number_of_processes)
{
    int time_ref = 0;

    printf("\n");
    for (int count = 0; count < number_of_processes; count++) {
        printf("| P%d\t", processes[count].process_number);
    }
    printf("|\n");

    for (int count = 0; count < number_of_processes; count++) {

        printf("%d\t", time_ref);

        time_ref += processes[count].burst_time;
        processes[count].CT = time_ref;
        processes[count].TAT = processes[count].CT - processes[count].arrival_time;
        processes[count].WT = processes[count].TAT - processes[count].burst_time;
    }
    printf("%d", time_ref);

}

/******************************************************************************
* \brief  Print the CT, TAT, WT
*
* \param[in] number_of_processes: total number of process entered by user
*
* *****************************************************************************/
void print_CT_TAT_WT(int number_of_processes){

    printf("\n\nProcess | CT\t| TAT\t| WT");

    for (int count = 0; count < number_of_processes; count++) {

        for (int j = 0; j < number_of_processes; j++) {

            if (processes[j].process_number == count+1) {

                printf("\nP%d\t| %d\t| %d\t| %d", processes[j].process_number,
                                            processes[j].CT,
                                            processes[j].TAT,
                                            processes[j].WT);
                break;
            }
        }
    }
}

/******************************************************************************
* \brief  Print average TAT
*
* \param[in] number_of_processes: total number of process entered by user
*
* *****************************************************************************/
void print_ATAT(int number_of_processes){
    float avg = 0;
    int sum = 0;

    for (int count = 0; count < number_of_processes; count ++) {
        sum +=processes[count].TAT;
    }

    avg = (float)((float)sum/number_of_processes);

    printf("\n\nAverage Turnaround Time (ATAT): %.2f", avg);
}

/******************************************************************************
* \brief  Print the average WT
*
* \param[in] number_of_processes: total number of process entered by user
*
* *****************************************************************************/
void print_AWT(int number_of_processes){
    
    float avg = 0;
    int sum = 0;

    for (int count = 0; count < number_of_processes; count ++) {

        sum +=processes[count].WT;
    }

    avg = (float)((float)sum/number_of_processes);
    printf("\nAverage Waiting Time (AWT): %.2f", avg);
}

/******************************************************************************
* \brief  Print the throughput
*
* \param[in] number_of_processes: total number of process entered by user
*
* *****************************************************************************/
void print_throughput(int number_of_processes){
    
    float throughput = 0;
    int max_CT = 0;

    for (int count = 0; count < number_of_processes; count ++) {

        if(max_CT  < processes[count].CT) {
            max_CT = processes[count].CT;
        }
    }

    //throughput = number of processes / maximum CT - arrival time of first process
    throughput = (float)((float)number_of_processes/((float)max_CT - processes[0].arrival_time));

    printf("\nThroughput: %.3f", throughput);
}


int main()
{
    int number_of_processes = 0;
    int i = 0;
    
    printf("Number of processes: ");
    scanf("%d", &number_of_processes);

    if ((number_of_processes < 1) || (number_of_processes > 100)) {
       printf("\nERROR: Number of processes should be a digit and inbetween 1 & 100 Exitting...");
       return -1; 
    }
    
    //Lets alloc memory so that we can fill & access the info using array
    processes = (struct node*)calloc(number_of_processes, sizeof(struct node));
    if(processes == NULL) {
        printf("ERROR: Note able to alloc memory. Exitting...");
    }
    
    printf("Process   Arrival Time     Burst Time\n");

    while(number_of_processes--) {
        
        int arrival_time = 0;
        int burst_time = 0;

        printf("%s%d\t", "P", i+1);
        scanf("%d", &arrival_time);

        if(arrival_time < 0) {
           printf("\nArrival Time could not be zero. Exitting...");
           return -1;
        }

        scanf("%d", &burst_time);
        if(burst_time < 0) {
           printf("\nBurst Time could not be zero. Exitting...");
           return -1;
        }

        processes[i].process_number = i+1;
        processes[i].arrival_time = arrival_time;
        processes[i].burst_time = burst_time;

        i++;
    }

    number_of_processes = i;

    // Arrange all the processes in the array accoring to the FCFS scheduling
    do_FCFS_scheduling(processes, number_of_processes);

    print_gantt_chart(number_of_processes);
    print_CT_TAT_WT(number_of_processes);
    print_ATAT(number_of_processes);
    print_AWT(number_of_processes);
    print_throughput(number_of_processes);

    return 0;
}

