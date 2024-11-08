#include <unistd.h>
#include <stdio.h>

int main()
{
    int child_to_parent_pipe[2];
    int parent_to_child_pipe[2];

    if (pipe(child_to_parent_pipe) == -1)
    {
        perror("Error opening child-to-parent pipe");
        return 1;
    }
    if (pipe(parent_to_child_pipe) == -1)
    {
        perror("Error opening parent-to-child pipe");
        return 2;
    }

    int id = fork();
    if (id == -1)
    {
        perror("Error during fork");
        return 3;
    }

    if (id == 0) // Child process
    {
        close(child_to_parent_pipe[0]); // Close read end of child-to-parent pipe
        close(parent_to_child_pipe[1]); // Close write end of parent-to-child pipe

        int child_send;
        int child_receive;
        printf("Please input a number to be sent to parent process:\n");
        if (scanf("%d", &child_send) != 1) // Check if scanf reads an integer
        {
            fprintf(stderr, "Error: Invalid input from child\n");
            return 4;
        }

        if (write(child_to_parent_pipe[1], &child_send, sizeof(int)) == -1)
        {
            perror("Error writing to pipe from child");
            return 5;
        }
        close(child_to_parent_pipe[1]); // End of writing

        if (read(parent_to_child_pipe[0], &child_receive, sizeof(int)) == -1)
        {
            perror("Error reading from pipe in child");
            return 6;
        }
        close(parent_to_child_pipe[0]); // End of reading

        printf("Received %d from parent\n", child_receive);
    }
    else // Parent process
    {
        close(child_to_parent_pipe[1]); // Close write end of child-to-parent pipe
        close(parent_to_child_pipe[0]); // Close read end of parent-to-child pipe

        int parent_receive;
        int parent_send;
        printf("Please input a number to send to child:\n");
        if (scanf("%d", &parent_send) != 1) // Check if scanf reads an integer
        {
            fprintf(stderr, "Error: Invalid input from parent\n");
            return 7;
        }

        if (write(parent_to_child_pipe[1], &parent_send, sizeof(int)) == -1)
        {
            perror("Error writing to pipe from parent");
            return 8;
        }
        close(parent_to_child_pipe[1]); // End of writing

        if (read(child_to_parent_pipe[0], &parent_receive, sizeof(int)) == -1)
        {
            perror("Error reading from pipe in parent");
            return 9;
        }
        close(child_to_parent_pipe[0]); // End of reading

        printf("Got %d from child process\n", parent_receive);
    }

    return 0;
}
