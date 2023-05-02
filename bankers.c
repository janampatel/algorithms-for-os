#include <stdio.h>
#include <stdlib.h>
int main()  
{  
    FILE *fp;
    int **alloc, **max, *avail, n, m, i, j, k;
    
    // open the file for reading
    fp = fopen("input_bankers.txt", "r");
    
    // read the number of rows and columns from the file
    fscanf(fp, "%d %d", &n, &m);

    // allocate memory for the 2D array dynamically
    alloc = (int **) malloc(n * sizeof(int *));
    for (i = 0; i < n; i++) {
        alloc[i] = (int *) malloc(m * sizeof(int));
    }
    
    // read the elements of the array from the file
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            fscanf(fp, "%d", &alloc[i][j]);
        }
    }

    max = (int **) malloc(n * sizeof(int *));
    for (i = 0; i < n; i++) {
        max[i] = (int *) malloc(m * sizeof(int));
    }
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            fscanf(fp, "%d", &max[i][j]);
        }
    }
  
    avail = (int *) malloc(m * sizeof(int));
    
    for (i = 0; i < m; i++) {
        fscanf(fp, "%d", &avail[i]);
    }
    
    // close the file
    fclose(fp);
  
    int f[n], ans[n], ind = 0;  
    for (k = 0; k < n; k++)  
    {  
        f[k] = 0;  
    }  
    int need[n][m];  
    for (i = 0; i < n; i++)  
    {  
        for (j = 0; j < m; j++)  
            need[i][j] = max[i][j] - alloc[i][j];  
    }  
    int y = 0;  
    for (k = 0; k < 5; k++)  
    {  
        for (i = 0; i < n; i++)  
        {  
            if (f[i] == 0)  
            {  
                int flag = 0;  
                for (j = 0; j < m; j++)  
                {  
                    if (need[i][j] > avail[j])  
                    {  
                        flag = 1;  
                        break;  
                    }  
                }  
                if (flag == 0)  
                {  
                    ans[ind++] = i;  
                    for (y = 0; y < m; y++)  
                        avail[y] += alloc[i][y];  
                    f[i] = 1;  
                }  
            }  
        }  
    }  
    int flag = 1;  
    for (int i = 0; i < n; i++)  
    {  
        if (f[i] == 0)  
        {  
            flag = 0;  
            printf("The following system is not safe");  
            break;  
        }  
    }  
    if (flag == 1)  
    {  
        printf("Following is the SAFE Sequence\n");  
        for (i = 0; i < n - 1; i++)  
            printf(" P%d ->", ans[i]);  
        printf(" P%d", ans[n - 1]);  
    }  
    return (0);  
} 