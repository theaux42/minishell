/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:27:21 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/01 03:31:30 by tbabou           ###   ########.fr       */
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
    char **argv;
    int argc;
    t_token *token;
    int i;

    // Compter le nombre de tokens de type COMMAND ou ARGUMENT
    argc = 0;
    token = command->tokens;
    while (token)
    {
        if (token->type == COMMAND || token->type == ARGUMENT)
            argc++;
        token = token->next;
    }

    // Allouer le tableau argv
    argv = malloc(sizeof(char *) * (argc + 1));
    if (!argv)
        return (-1); // Gérer l'erreur d'allocation si nécessaire

    // Remplir le tableau argv avec les valeurs des tokens
    token = command->tokens;
    i = 0;
    while (token)
    {
        if (token->type == COMMAND || token->type == ARGUMENT)
        {
            argv[i] = token->value; // Utiliser directement la valeur du token
            i++;
        }
        token = token->next;
    }
    argv[i] = NULL;

    pid = fork();
    if (pid < 0)
    {
        free(argv);
        return (-1);
    }
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
        // Les redirections sont gérées ailleurs
        execve(cmd, argv, env);
        exit_error("Execve");
    }
    free(argv); // Libérer argv dans le processus parent
    return (pid); // Le parent retourne le PID de l'enfant
}

int exec_cmd(t_command *command, char **env)
{
    char *cmd;
    int pid;
    t_token *tokens;

    tokens = command->tokens;
    // Trouver le premier token de type COMMAND
    while (tokens && tokens->type != COMMAND)
        tokens = tokens->next;
    if (!tokens)
        return (CMD_NOT_FOUND);
    cmd = get_full_cmd(tokens->value); // get_full_cmd prend le nom de la commande
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
    extern char **environ;
    t_command *current;
    int prev_fd = -1;

    init_pipes(commands);
    current = commands;
    while (current)
    {
        current->prev_pipe = prev_fd;
        current->pid = exec_cmd(current, environ);
        if (current->pid == CMD_NOT_FOUND)
        {
            t_token *tok = current->tokens;
            while (tok && tok->type != COMMAND)
                tok = tok->next;
            if (tok)
                printf("Command not found: %s\n", tok->value);
            else
                printf("Command not found\n");
        }
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