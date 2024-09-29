/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:53:44 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/29 17:29:41 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_full_cmd(t_command *command)
{
    char *path;
    char **paths;
    int i;
    
    i = 0;
    if (command->argv[0][0] == '/')
    {
        if (access(command->argv[0], F_OK) == 0)
            return (command->argv[0]);
        return (NULL);
    }
    path = getenv("PATH");
    paths = ft_split(path, ':');
    while (paths[i++])
    {
        path = ft_strjoin(paths[i], "/");
        paths[i] = ft_strjoin(path, command->argv[0]);
    }
    free(path);
    i = 0;
    while (paths[i++])
        if (access(paths[i], F_OK) == 0)
            return (paths[i]);
    return (NULL);
}

void free_minishell(void)
{
    printf("free minishell\n");
}

void set_default_values(t_command *command)
{
    command->argv = NULL;
    command->next = NULL;
    command->redirections = NULL;
    command->is_last = 0;
    command->is_builtin = 0;
    command->is_absolute = 0;
    command->pipes[0] = -1;
    command->pipes[1] = -1;
    command->prev_pipe = -1;
    command->pid = -1;
}

void exit_error(char *msg)
{
    perror(msg);
    free_minishell();
    exit(EXIT_FAILURE);
}