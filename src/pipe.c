#include <unistd.h>
#include <stdio.h>
int main()
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("error with opening pipe\n");
        return 1;
    }
    int id = fork();
    //send from child to parent
    if (id == 0)
    {
        close(fd[0]);// close read end
        int x;
        printf("please input a number to be sent to parent process:\n");
        scanf("%d", &x);
        write(fd[1], &x, sizeof(int));
        close(fd[1]);//indicating the end of writing
        //in the writing section the variable must be initilaised.
    }
    else
    {
        close(fd[1]);//close the unused end if the pipe here
        int y;
        read (fd[0], &y, sizeof(int));
        close (fd[0]);
        printf("got %d from child process\n", y);
    }
    return (0);
}