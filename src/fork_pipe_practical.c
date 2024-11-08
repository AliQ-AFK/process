#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char **argv)
{
    int arr[] = {1, 2 , 3, 4, 1, 2, 6};
    int start, end; // an inde for the proccess to sort.
    int arr_size = sizeof(arr) / sizeof(int);// size of arr = number of bytes only.
    int fd[2];
    if (pipe(fd) == -1)
        return (1);
    int id = fork();
    if (id == -1)
        return (2);
    if (id == 0)// child
    { 
        start = 0;// index 1
        end =(arr_size / 2); // this is floored,  
    }
    else
    {
        start = (arr_size / 2);// odd or even, will work as it is floored
        end = arr_size;
    }
    int sum = 0;
    int i;
    for (i = start; i < end; i++)
        sum += arr[i];
    printf("Calculated partial sum of the process: %d\n", sum);
    
    if (id == 0)
    {
        close (fd[0]);
        if (write (fd[1], &sum, sizeof(sum)))
            return (3);
        close (fd[0]); 
    }
    else{
        int sum_child;
        close (fd[1]);
        if (read (fd[0], &sum_child, sizeof(sum_child)))
            return (4);
        close (fd[0]);
        int total_sum = sum + sum_child;
        printf("the sum from parent and child is : %d", total_sum);
        wait(NULL);
    }
    return (0);
}