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
        close(fd[1]);// close read end
        int x;
        read(fd[0], &x, sizeof(int));
        close(fd[0]);//indicating the end of writing
        printf("got %d from parent process\n", x);
    }
    else
    {
        close(fd[1]);//close the unused end if the pipe here
        int y;
        printf("please input a number to be sent to child process:\n");
        scanf("%d", &y);
        write(fd[0], &y, sizeof(int));
        close (fd[0]);
    }
    return (0);
}