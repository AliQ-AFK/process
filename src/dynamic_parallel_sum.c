#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int n;
    printf("Enter the number of child processes: ");//get number of processes from user.
    scanf("%d", &n);

    int arr_size;
    printf("Enter the size of the array: ");//get size of array from user.
    scanf("%d", &arr_size);

    int *arr = (int *)malloc(arr_size * sizeof(int));
    if (arr == NULL)
    {
        perror("malloc");
        return 1;
    }

    printf("Enter %d elements of the array: ", arr_size);//user enters the elements.
    for (int i = 0; i < arr_size; i++)
    {
        scanf("%d", &arr[i]);
    }

    if (n > arr_size)
    {
        printf("\033[33mWarning: Number of processes is greater than the number of integers in the array.\n");
        printf("Extra processes will have a sum of 0.\033[0m\n");
        n = arr_size;//handles extra processes
    }

    int fd[n][2];//2d pipe
    for (int i = 0; i < n; i++)// n of processes == pipes
    {
        if (pipe(fd[i]) == -1)
        {
            perror("pipe");
            free(arr);
            return 1;
        }
    }

    for (int i = 0; i < n; i++)//calclua tion and forking is parallel = less overhead and simpler code
    {
        //first loop for forking.
        int id = fork();
        if (id == -1)
        {
            perror("fork");
            free(arr);
            return 2;
        }

        if (id == 0)//access child
        {
            close(fd[i][0]);//each child process has its pipe to parent
            int start = (i * arr_size) / n;
            int end = ((i + 1) * arr_size) / n;
            int sum = 0;
            for (start; start < end; start++)//calculates the sum
            {
                sum += arr[j];
            }
            write(fd[i][1], &sum, sizeof(sum));
            close(fd[i][1]);
            free(arr);
            return 0;//after writing and freeing child exits
        }
    }
    printf("\n\033[34mChild ID | Partial Sum\033[0m\n");
    printf("\033[34m---------------------\033[0m\n");

    int total_sum = 0;
    for (int i = 0; i < n; i++)
    {
        close(fd[i][1]);
        int sum;
        read(fd[i][0], &sum, sizeof(sum));
        printf("\033[36m%8d | %10d\033[0m\n", i, sum);
        total_sum += sum;
        close(fd[i][0]);
    }

    printf("\033[34m---------------------\033[0m\n");
    printf("\033[32mTotal sum: %d\033[0m\n", total_sum);

    free(arr);
    return 0;
}
