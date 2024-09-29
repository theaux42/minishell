/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:27:21 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/29 17:23:19 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void init_pipes(t_command *commands)
{
    t_command *current;

    current = commands;
    while (current)
    {
        if (current->next)
        {
            if (pipe(current->pipes) == -1)
                exit_error("pipe");
        }
        else
        {
            current->pipes[0] = -1;
            current->pipes[1] = -1;
        }
        current = current->next;
    }
}

int execution(char *cmd, t_command *command, char **env)
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
        return (-1);
    else if (pid == 0)
    {
        if (command->prev_pipe != -1)
        {
            if (dup2(command->prev_pipe, STDIN_FILENO) == -1)
                exit_error("dup2 input_fd");
            close(command->prev_pipe);
        }
        if (command->pipes[1] != -1)
        {
            if (dup2(command->pipes[1], STDOUT_FILENO) == -1)
                exit_error("dup2 output_fd");
            close(command->pipes[1]);
        }
        if (command->pipes[0] != -1)
            close(command->pipes[0]);
        execve(cmd, command->argv, env);
        exit_error("Execve");
    }
    return (pid); // Le parent retourne le PID de l'enfant
}

int exec_cmd(t_command *command, char **env)
{
    char *cmd;
    int pid;

    cmd = get_full_cmd(command);
    if (!cmd)
         return (CMD_NOT_FOUND);
    pid = execution(cmd, command, env);
    free(cmd);
    return (pid);
}

void wait_for_children(t_command *commands)
{
    t_command *current;
    int status;

    current = commands;
    while (current)
    {
        waitpid(current->pid, &status, 0);
        current = current->next;
    }
}

void execute_command(t_command *commands)
{
    char *envp[] = {"TERM=xterm-256color", NULL};
    t_command *current;
    int prev_fd = -1;

    init_pipes(commands);
    current = commands;
    while (current)
    {
        current->prev_pipe = prev_fd;
        current->pid = exec_cmd(current, envp);
        if (current->pid == CMD_NOT_FOUND)
            printf("Command not found: %s\n", current->argv[0]);
        if (current->pid == -1)
            exit(EXIT_FAILURE);
        if (prev_fd != -1)
            close(prev_fd);
        if (current->pipes[1] != -1)
            close(current->pipes[1]);
        prev_fd = current->pipes[0];
        current = current->next;
    }
    if (prev_fd != -1)
        close(prev_fd);
    wait_for_children(commands);
}


// void execute_command(t_command *command)
// {
//     char *envp[] = {"TERM=xterm-256color", NULL}; // Need more understanding
//     while (command)
//     {
//         exec_cmd(command, envp);
//         ft_freesplit(command->argv);
//         command = command->next;
//     }
// }