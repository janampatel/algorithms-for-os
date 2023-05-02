#include <stdio.h>
#include <stdlib.h>

void implimentWorstFit(int blockSize[], int blocks, int processSize[], int processes)
{
    int allocation[processes];
    int occupied[blocks];
    for (int i = 0; i < processes; i++)
    {
        allocation[i] = -1;
    }
    for (int i = 0; i < blocks; i++)
    {
        occupied[i] = 0;
    }
    for (int i = 0; i < processes; i++)
    {
        int indexPlaced = -1;

        for (int j = 0; j < blocks; j++)
        {
            if (blockSize[j] >= processSize[i] && !occupied[j])
            {
                if (indexPlaced == -1)
                    indexPlaced = j;
                else if (blockSize[indexPlaced] < blockSize[j])
                    indexPlaced = j;
            }
        }
        if (indexPlaced != -1)
        {
            allocation[i] = indexPlaced;
            occupied[indexPlaced] = 1;
            blockSize[indexPlaced] -= processSize[i];
        }
    }
    printf("\nProcess No.\tProcess Size\tBlock no.\n");
    for (int i = 0; i < processes; i++)
    {
        printf("%d \t\t %d \t\t", i + 1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

void implimentBestFit(int blockSize[], int blocks, int processSize[], int proccesses)
{
    int allocation[proccesses];
    int occupied[blocks];

    for (int i = 0; i < proccesses; i++)
    {
        allocation[i] = -1;
    }
    for (int i = 0; i < blocks; i++)
    {
        occupied[i] = 0;
    }

    for (int i = 0; i < proccesses; i++)
    {
        int indexPlaced = -1;
        for (int j = 0; j < blocks; j++)
        {
            if (blockSize[j] >= processSize[i] && !occupied[j])
            {
                if (indexPlaced == -1)
                    indexPlaced = j;

                else if (blockSize[j] < blockSize[indexPlaced])
                    indexPlaced = j;
            }
        }

        if (indexPlaced != -1)
        {
            allocation[i] = indexPlaced;

            occupied[indexPlaced] = 1;
        }
    }
    printf("\nProcess No.\tProcess Size\tBlock no.\n");
    for (int i = 0; i < proccesses; i++)
    {
        printf("%d \t\t %d \t\t", i + 1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

void implimentFirstFit(int blockSize[], int m, int processSize[], int n)
{
    int i, j;
    int allocation[n];
    for (i = 0; i < n; i++)
    {
        allocation[i] = -1;
    }
    for (i = 0; i < n; i++) // here, n -> number of processes
    {
        for (j = 0; j < m; j++) // here, m -> number of blocks
        {
            if (blockSize[j] >= processSize[i])
            {
                // allocating block j to the ith process
                allocation[i] = j;
                // Reduce available memory in this block.
                blockSize[j] -= processSize[i];
                break; // go to the next process in the queue
            }
        }
    }
    printf("\nProcess No.\tProcess Size\tBlock no.\n");
    for (int i = 0; i < n; i++)
    {
        printf(" %d\t\t %d \t\t", i + 1,processSize[i]);
        if (allocation[i] != -1)
            printf("%d", allocation[i] + 1);
        else
            printf("Not Allocated");
        printf("\n");
    }
}

int main()
{
    int blockSize[4];
    int processSize[4];
    FILE *file1 = fopen("blo_size.txt", "r");
    for (int i = 0; i < 4; i++)
    {
        fscanf(file1, "%d\t", &blockSize[i]);
    }

    fclose(file1);
    FILE *file2 = fopen("pro_size.txt", "r");
    for (int i = 0; i < 4; i++)
    {
        fscanf(file2, "%d\t", &processSize[i]);
    }
    fclose(file2);

    int blocks = sizeof(blockSize) / sizeof(blockSize[0]);
    int processes = sizeof(processSize) / sizeof(processSize[0]);
    printf("Worst fit:\n");
    implimentWorstFit(blockSize, blocks, processSize, processes);
    printf("Best fit:\n");
    implimentBestFit(blockSize, blocks, processSize, processes);
    printf("First fit:\n");
    implimentFirstFit(blockSize, blocks, processSize, processes);
    return 0;
}