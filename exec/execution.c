/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:27:21 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/29 01:55:01 by tbabou           ###   ########.fr       */
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
            else
                printf("Pipe créé : [lecture=%d, écriture=%d]\n", current->pipes[0], current->pipes[1]);
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
        // Enfant

        // Redirection de l'entrée standard
        if (command->prev_pipe != -1)
        {
            if (dup2(command->prev_pipe, STDIN_FILENO) == -1)
                exit_error("dup2 input_fd");
            printf("Enfant PID %d : dup2 %d vers STDIN_FILENO\n", getpid(), command->prev_pipe);
            close(command->prev_pipe);
            printf("Enfant PID %d : fermé prev_pipe %d\n", getpid(), command->prev_pipe);
        }

        // Redirection de la sortie standard
        if (command->pipes[1] != -1)
        {
            if (dup2(command->pipes[1], STDOUT_FILENO) == -1)
                exit_error("dup2 output_fd");
            printf("Enfant PID %d : dup2 %d vers STDOUT_FILENO\n", getpid(), command->pipes[1]);
            close(command->pipes[1]);
            printf("Enfant PID %d : fermé pipes[1] %d\n", getpid(), command->pipes[1]);
        }

        // Fermeture des descripteurs inutilisés
        if (command->pipes[0] != -1)
        {
            close(command->pipes[0]);
            printf("Enfant PID %d : fermé pipes[0] %d\n", getpid(), command->pipes[0]);
        }

        execve(cmd, command->argv, env);
        exit_error("Execve");
    }
    return (pid); // Le parent retourne le PID de l'enfant
}

int exec_cmd(t_command *command, char **env)
{
    char *cmd;
    int pid;

    cmd = get_full_cmd(command->argv[0]);
    if (!cmd)
    {
        ft_printf("Command not found: %s\n", command->argv[0]);
        return (-1);
    }
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
        printf("Processus enfant PID %d terminé avec status %d\n", current->pid, status);
        current = current->next;
    }
}

void execute_command(t_command *commands)
{
    extern char **environ; // Utilisez l'environnement actuel
    t_command *current;
    int prev_fd = -1;

    init_pipes(commands);

    current = commands;
    while (current)
    {
        current->prev_pipe = prev_fd;

        current->pid = exec_cmd(current, environ);
        if (current->pid == -1)
            exit(EXIT_FAILURE);

        // Fermeture des descripteurs inutilisés dans le parent
        if (prev_fd != -1)
        {
            close(prev_fd);
            printf("Parent : fermé prev_fd %d\n", prev_fd);
        }

        // Fermer le descripteur d'écriture du pipe actuel dans le parent
        if (current->pipes[1] != -1)
        {
            close(current->pipes[1]);
            printf("Parent : fermé pipes[1] %d\n", current->pipes[1]);
        }

        // Préparer l'entrée pour la prochaine commande
        prev_fd = current->pipes[0];

        current = current->next;
    }

    // Fermer le dernier descripteur de lecture s'il n'est pas STDIN
    if (prev_fd != -1)
    {
        close(prev_fd);
        printf("Parent : fermé dernier prev_fd %d\n", prev_fd);
    }

    // Attendre tous les processus enfants
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