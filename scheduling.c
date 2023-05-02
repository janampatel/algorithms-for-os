#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

void FCFS();
void SJF();
void RR();
void PS();
void SRTF();
void LRTF();

#define Max_Process 100
int main()
{
    int n;
    printf("\n\nEnter the choice of scheduling algorithm that you want:\n");
    printf("1. First-Come First-Served\n");
    printf("2. Shortest Job First\n");
    printf("3. Round Robin Scheduling\n");
    printf("4. Priority Scheduling\n");
    printf("5. Shortest Remaining Time First\n");
    printf("6. Longest Remaining Time First\n");
    printf("Choice: ");
    scanf("%d", &n);
    switch (n)
    {
    case 1:
        FCFS();
        break;
    case 2:
        SJF();
        break;
    case 3:
        RR();
        break;
    case 4:
        PS();
        break;
    case 5:
        SRTF();
        break;
    case 6:
        LRTF();
        break;
    default:
        break;
    }
}
struct Process
{
    int id;
    int arrival_time;
    int burst_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
};
int comparefcfs(const void *a, const void *b)
{
    return ((struct Process *)a)->arrival_time - ((struct Process *)b)->arrival_time;
}
void FCFS()
{
    FILE *input_file = fopen("input_fcfs.txt", "r");
    FILE *output_file = fopen("output_fcfs.txt", "w");
    fprintf(output_file, "First Come First Served Scheduling Algorithm:\n\n");
    int num_processes;
    fscanf(input_file, "%d", &num_processes);
    struct Process *processes = (struct Process *)malloc(num_processes * sizeof(struct Process));
    for (int i = 0; i < num_processes; i++)
    {
        fscanf(input_file, "%d %d %d", &processes[i].id, &processes[i].arrival_time, &processes[i].burst_time);
    }
    fclose(input_file);
    qsort(processes, num_processes, sizeof(struct Process), comparefcfs);
    int current_time = 0;
    for (int i = 0; i < num_processes; i++)
    {
        if (processes[i].arrival_time > current_time)
        {
            current_time = processes[i].arrival_time;
        }
        processes[i].finish_time = current_time + processes[i].burst_time;
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        if (processes[i].waiting_time < 0)
        {
            processes[i].waiting_time = 0;
        }
        current_time = processes[i].finish_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
    }
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < num_processes; i++)
    {
        avg_waiting_time += processes[i].waiting_time;
        avg_turnaround_time += processes[i].turnaround_time;
    }
    avg_waiting_time /= num_processes;
    avg_turnaround_time /= num_processes;
    fprintf(output_file, "Average turnaround time: %.2f\n", avg_turnaround_time);
    fprintf(output_file, "Average waiting time: %.2f\n", avg_waiting_time);
    fprintf(output_file, "Gantt Chart:\n");
    for (int i = 0; i < num_processes; i++)
    {
        for (int j = 0; j < processes[i].burst_time; j++)
        {
            fprintf(output_file, "| P%d ", processes[i].id);
        }
    }
    fprintf(output_file, "|\n");
    fclose(output_file);
    free(processes);
    return;
}
struct processsjf
{
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int finish_time;
    int turnaround_time;
    int waiting_time;
};
int comparesjf(const void *p1, const void *p2)
{
    struct processsjf *a = (struct processsjf *)p1;
    struct processsjf *b = (struct processsjf *)p2;
    if (a->burst_time < b->burst_time)
    {
        return -1;
    }
    else if (a->burst_time == b->burst_time)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void SJF()
{
    FILE *input_file, *output_file;
    input_file = fopen("input_sjf.txt", "r");
    output_file = fopen("output_sjf.txt", "w");
    fprintf(output_file, "Shortest Job First Scheduling Algorithm:\n\n");
    // Read input from the file
    int num_processes;
    fscanf(input_file, "%d", &num_processes);
    // Create an array of processes and initialize them
    struct processsjf processes[num_processes];
    for (int i = 0; i < num_processes; i++)
    {
        fscanf(input_file, "%d %d %d", &processes[i].process_id, &processes[i].arrival_time,&processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }
    // Sort the processes based on their burst time
    qsort(processes, num_processes, sizeof(struct processsjf), comparesjf);
    int current_time = 0;
    int completed_processes = 0;
    // Create an array to store the order of execution
    int execution_sequence[1000], num_executions = 0;
    // Run the Shortest Job First algorithm
    while (completed_processes < num_processes)
    {
        int shortest_job_index = -1;
        int shortest_job_burst_time = 100000;
        for (int i = 0; i < num_processes; i++)
        {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0)
            {
                if (processes[i].burst_time < shortest_job_burst_time)
                {
                    shortest_job_index = i;
                    shortest_job_burst_time = processes[i].burst_time;
                }
            }
        }
        if (shortest_job_index == -1)
        {
            current_time++;
            execution_sequence[num_executions++] = -1;
        }
        else
        {
            current_time += processes[shortest_job_index].remaining_time;
            processes[shortest_job_index].remaining_time = 0;
            processes[shortest_job_index].finish_time = current_time;
            completed_processes++;
            execution_sequence[num_executions++] = processes[shortest_job_index].process_id;
        }
    }
    // Calculate turnaround time and waiting time for each process
    int total_turnaround_time = 0, total_waiting_time = 0;
    for (int i = 0; i < num_processes; i++)
    {
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
    }
    // Print the average turnaround time and waiting time
    float avg_turnaround_time = (float)total_turnaround_time / num_processes;
    float avg_waiting_time = (float)total_waiting_time / num_processes;
    fprintf(output_file, "Average turnaround time: %.2f\n", avg_turnaround_time);
    fprintf(output_file, "Average waiting time: %.2f\n", avg_waiting_time);
    // Print the Gantt chart
    fprintf(output_file, "Gantt Chart:\n");
    for (int i = 0; i < num_processes; i++)
    {
        for (int j = 0; j < processes[i].burst_time; j++)
        {
            fprintf(output_file, "P%d |", processes[i].process_id);
        }
    }
    fprintf(output_file, "\n");
    fclose(output_file);
    free(processes);
    return;
}
// Structure to represent a process
struct processrr
{
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int finish_time;
    int turnaround_time;
    int waiting_time;
};
void RR()
{
    FILE *input_file, *output_file;
    input_file = fopen("input_rr.txt", "r");
    output_file = fopen("output_rr.txt", "w");
    fprintf(output_file, "Round Robin Algorithm:\n\n");
    // Read input from the file
    int num_processes, quant_time;
    fscanf(input_file, "%d %d", &num_processes, &quant_time);
    // Create an array of processes and initialize them
    struct processrr processes[num_processes];
    for (int i = 0; i < num_processes; i++)
    {
        fscanf(input_file, "%d %d %d", &processes[i].process_id, &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }
    int current_time = 0;
    int completed_processes = 0;
    // Create an array to store the order of execution
    int execution_sequence[1000], num_executions = 0;
    // Run the Round Robin algorithm
    while (completed_processes < num_processes)
    {
        int flag = 0;
        for (int i = 0; i < num_processes; i++)
        {
            if (processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time)
            {
                flag = 1;
                if (processes[i].remaining_time <= quant_time)
                {
                    current_time += processes[i].remaining_time;
                    processes[i].remaining_time = 0;
                    processes[i].finish_time = current_time;
                    completed_processes++;
                    execution_sequence[num_executions++] = processes[i].process_id;
                }
                else
                {
                    current_time += quant_time;
                    processes[i].remaining_time -= quant_time;
                    execution_sequence[num_executions++] = processes[i].process_id;
                }
            }
        }
        if (flag == 0)
        {
            current_time++;
            execution_sequence[num_executions++] = -1;
        }
    }
    // Calculate turnaround time and waiting time for each process
    int total_turnaround_time = 0, total_waiting_time = 0;
    for (int i = 0; i < num_processes; i++)
    {
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
    }
    // Print the average turnaround time and waiting time
    float avg_turnaround_time = (float)total_turnaround_time / num_processes;
    float avg_waiting_time = (float)total_waiting_time / num_processes;
    fprintf(output_file, "Average turnaround time: %.2f\n", avg_turnaround_time);
    fprintf(output_file, "Average waiting time: %.2f\n", avg_waiting_time);
    // Print the Gantt Chart
    fprintf(output_file, "Gantt Chart:\n");
    for (int i = 0; i < num_executions; i++)
    {
        if (execution_sequence[i] == -1)
        {
            fprintf(output_file, "--");
        }
        else
        {
            fprintf(output_file, "P%d |", execution_sequence[i]);
        }
        fprintf(output_file, " ");
    }
    fprintf(output_file, "\n");
    fclose(output_file);
    free(processes);
    return;
}
// Structure to represent a process
struct processps
{
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int finish_time;
    int turnaround_time;
    int waiting_time;
    int priority;
};
// Function to compare two processes based on their priority
int compareps(const void *p1, const void *p2)
{
    struct processps *a = (struct processps *)p1;
    struct processps *b = (struct processps *)p2;
    if (a->priority < b->priority)
    {
        return -1;
    }
    else if (a->priority == b->priority)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void PS()
{
    FILE *input_file, *output_file;
    input_file = fopen("input_ps.txt", "r");
    output_file = fopen("output_ps.txt", "w");
    fprintf(output_file, "Priority Scheduling Algorithm:\n\n");
    // Read input from the file
    int num_processes;
    fscanf(input_file, "%d", &num_processes);
    // Create an array of processes and initialize them
    struct processps processes[num_processes];
    for (int i = 0; i < num_processes; i++)
    {
        fscanf(input_file, "%d %d %d %d", &processes[i].process_id, &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority);
        processes[i].remaining_time = processes[i].burst_time;
    }
    // Sort the processes based on their priority
    qsort(processes, num_processes, sizeof(struct processps), compareps);
    int current_time = 0;
    int completed_processes = 0;
    // Create an array to store the order of execution
    int execution_sequence[1000], num_executions = 0;
    // Run the Priority Scheduling algorithm
    while (completed_processes < num_processes)
    {
        int highest_priority_index = -1;
        int highest_priority = -1;
        for (int i = 0; i < num_processes; i++)
        {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0)
            {
                if (processes[i].priority > highest_priority)
                {
                    highest_priority_index = i;
                    highest_priority = processes[i].priority;
                }
            }
        }
        if (highest_priority_index == -1)
        {
            current_time++;
            execution_sequence[num_executions++] = -1;
        }
        else
        {
            current_time += processes[highest_priority_index].remaining_time;
            processes[highest_priority_index].remaining_time = 0;
            processes[highest_priority_index].finish_time = current_time;
            completed_processes++;
            execution_sequence[num_executions++] = processes[highest_priority_index].process_id;
        }
    }
    // Calculate turnaround time and waiting time for each process
    int total_turnaround_time = 0, total_waiting_time = 0;
    for (int i = 0; i < num_processes; i++)
    {
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
    }
    // Print the average turnaround time and waiting time
    float avg_turnaround_time = (float)total_turnaround_time / num_processes;
    float avg_waiting_time = (float)total_waiting_time / num_processes;
    fprintf(output_file, "Average turnaround time: %.2f\n", avg_turnaround_time);
    fprintf(output_file, "Average waiting time: %.2f\n", avg_waiting_time);
    // Print the Gantt Chart
    fprintf(output_file, "Gantt Chart:\n");
    for (int i = 0; i < num_processes; i++)
    {
        for (int j = 0; j < processes[i].burst_time; j++)
        {
            fprintf(output_file, "P%d |", processes[i].process_id);
        }
    }
    fprintf(output_file, "\n");
    fclose(output_file);
    free(processes);
    return;
}
void SRTF()
{
    FILE *input_file, *output_file;
    input_file = fopen("input_srtf.txt", "r");
    output_file = fopen("output_srtf.txt", "w");
    fprintf(output_file, "Shortest Remaining Time First Scheduling Algorithm:\n\n");
    int i, j, current_time = 0, total_waiting_time = 0, total_turnaround_time = 0, remaining_processes = 0, shortest_process, shortest_burst_time = INT_MAX;
    int n;
    fscanf(input_file, "%d", &n);
    int arrival_time[n], burst_time[n], waiting_time[n], turnaround_time[n], process_id[n],
        remaining_time[n];
    for (i = 0; i < n; i++)
    {
        fscanf(input_file, "%d %d", &arrival_time[i], &burst_time[i]);
        remaining_time[i] = burst_time[i];
        process_id[i] = i + 1;
        remaining_processes++;
    }
    while (remaining_processes > 0)
    {
        shortest_process = -1;
        shortest_burst_time = INT_MAX;
        for (i = 0; i < n; i++)
        {
            if (arrival_time[i] <= current_time && remaining_time[i] < shortest_burst_time &&
                remaining_time[i] > 0)
            {
                shortest_process = i;
                shortest_burst_time = remaining_time[i];
            }
        }
        if (shortest_process == -1)
        {
            current_time++;
            continue;
        }
        fprintf(output_file, "| P%d |", process_id[shortest_process]);
        remaining_time[shortest_process]--;
        current_time++;
        if (remaining_time[shortest_process] == 0)
        {
            remaining_processes--;
            waiting_time[shortest_process] = current_time - arrival_time[shortest_process] -
                                             burst_time[shortest_process];
            turnaround_time[shortest_process] = current_time - arrival_time[shortest_process];
            total_waiting_time += waiting_time[shortest_process];
            total_turnaround_time += turnaround_time[shortest_process];
        }
    }
    fprintf(input_file, "\n\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++)
    {
        fprintf(output_file, "\nP[%d]\t\t%d\t\t%d\t\t%d", process_id[i], burst_time[i], waiting_time[i],
                turnaround_time[i]);
    }
    fprintf(output_file, "\nAverage waiting time: %.2f\n", (float)total_waiting_time / n);
    fprintf(output_file, "Average turnaround time: %.2f\n", (float)total_turnaround_time / n);
    fclose(output_file);
    return;
}
void LRTF()
{
    int n, total_waiting_time = 0, total_turnaround_time = 0, i, j;
    FILE *input_file, *output_file;
    input_file = fopen("input_lrtf.txt", "r");
    output_file = fopen("output_lrtf.txt", "w");
    fprintf(output_file, "Longest Remaining Time First Scheduling Algorithm:\n\n");
    fscanf(input_file, "%d", &n);
    int arrival_time[n];
    int burst_time[n];
    int remaining_time[n];
    int turnaround_time[n];
    int waiting_time[n];
    int finish_time[n];
    for (i = 0; i < n; i++)
    {
        fscanf(input_file, "%d %d", &arrival_time[i], &burst_time[i]);
        remaining_time[i] = burst_time[i];
    }
    int current_time = 0;
    int selected_process = -1;
    fprintf(output_file, "\n");
    while (1)
    {
        selected_process = -1;
        int longest_remaining_time = 0;
        for (int i = 0; i < n; i++)
        {
            if (arrival_time[i] <= current_time && remaining_time[i] > longest_remaining_time)
            {
                selected_process = i;
                longest_remaining_time = remaining_time[i];
            }
        }
        // If there are no more processes to execute, exit the loop
        if (selected_process == -1)
        {
            break;
        }
        // Update the Gantt chart
        fprintf(output_file, "| P%d ", selected_process);
        // Execute the selected process for 1 unit of time
        remaining_time[selected_process] -= 1;
        current_time += 1;
        // If the process has completed, record its completion time
        if (remaining_time[selected_process] == 0)
        {
            finish_time[selected_process] = current_time;
            turnaround_time[selected_process] = finish_time[selected_process] - arrival_time[selected_process];
            waiting_time[selected_process] = turnaround_time[selected_process] - burst_time[selected_process];
        }
    }
    fprintf(output_file, "|\n");
    // Print the results
    fprintf(output_file, "\nProcess\t\tBurstTime\tWaitingTime\tTurnaroundTime");
    for (int i = 0; i < n; i++)
    {
        fprintf(output_file, "\nP[%d]\t\t%d\t\t%d\t\t%d", i, burst_time[i], waiting_time[i],
                turnaround_time[i]);
    }
    for (int i = 0; i < n; i++)
    {
        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
    }
    fprintf(output_file, "\nAverage waiting time: %.2f\n", (float)total_waiting_time / n);
    fprintf(output_file, "Average turnaround time: %.2f\n", (float)total_turnaround_time / n);
    fclose(output_file);
    return;
}