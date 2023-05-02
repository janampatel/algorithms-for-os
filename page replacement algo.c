#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fp = fopen("Lab7text.txt", "r");
    if (!fp)
    {
        printf("Can't open file\n");
        return 0;
    }

    char p[1000];
    fgets(p, 1000, fp);
    int cnt = 0;
    for (int i = 0; i < strlen(p); i++)
    {
        if (p[i] == ',')
        {
            cnt++;
        }

        if (p[i] == '\n')
        {
            break;
        }
    }

    cnt++;

    fclose(fp);

    fp = fopen("Lab7text.txt", "r");
    if (!fp)
    {
        printf("Can't open file\n");
        return 0;
    }

    int arr[cnt];
    int y = 0;
    int idx = 0;
    char p2[1000];

    fgets(p2, 1000, fp);

    {
        // printf("%d \n",y);
        {
            int r = cnt - 1;
            printf("%d \n", y);
            char *token = strtok(p2, ",");
            int at = atoi(token);
            arr[idx] = at;
            idx++;
            while (r--)
            {
                token = strtok(NULL, ",");
                int at = atoi(token);
                arr[idx] = at;
                idx++;
            }
        }
    }
    fclose(fp);

    while (1)
    {
        printf("\n********** Main Menu **********\n");
        printf("1: FIFO\n");
        printf("2: LRU\n");
        printf("3: Exit\n");
        printf("Enter: ");
        int n1;
        scanf("%d", &n1);
        for (int i = 0; i < cnt; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");
        if (n1 == 3)
        {
            printf("\nThank You :)\n\n");
            return 0;
        }
        printf("Enter the number of frames: ");
        int n;
        scanf("%d", &n);
        if (n1 == 1)
        {

            printf("\n");

            int fr[n];

            for (int i = 0; i < n; i++)
            {
                fr[i] = -1;
            }

            int it = 0;
            int high = 0;
            for (int j = 0; j < cnt; j++)
            {
                int g = arr[j];
                int flag = 1;
                for (int i = 0; i < n; i++)
                {
                    if (g == fr[i])
                    {
                        flag = 0;
                        printf("H ");
                        high++;

                        break;
                    }
                }
                if (flag)
                {
                    fr[it] = g;
                    printf("F ");
                    it = (it + 1) % n;
                }
            }

            printf("\nNumber of Page hit: %d \n", high);
            printf("Number of Page miss: %d \n\n", cnt - high);
        }
        else if (n1 == 2)
        {
            int fr[n];
            int it = 0;
            int high = 0;
            int ck[n];
            for (int i = 0; i < n; i++)
            {
                fr[i] = -1;
                ck[i] = 0;
            }

            it = 0;
            high = 0;

            for (int j = 0; j < cnt; j++)
            {
                int g = arr[j];
                int flag = 1;
                for (int i = 0; i < n; i++)
                {
                    if (g == fr[i])
                    {
                        flag = 0;
                        printf("H ");
                        ck[i] = -1;
                        high++;
                        break;
                    }
                }

                if (flag)
                {
                    int yr = 0;
                    int yr1 = 0;
                    for (int i = 0; i < n; i++)
                    {
                        if (ck[i] > yr)
                        {
                            yr = ck[i];
                            yr1 = i;
                        }
                    }
                    fr[yr1] = g;
                    ck[yr1] = -1;
                    printf("F ");
                    // it=(it+1)%n;
                }
                // else
                {
                    for (int i = 0; i < n; i++)
                    {
                        ck[i] += 1;
                    }
                }
            }

            printf("\nNumber of Page hit: %d \n", high);
            printf("Number of Page miss: %d \n\n", cnt - high);
        }
        else if (n1 == 3)
        {
            printf("\nThank You :)\n");
            return 0;
        }
        else
        {
            printf("Invalid Input\n");
        }
    }
    return 0;
}