/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 22:41:56 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/29 22:42:03 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void get_cr_command(t_command *commands, char *current_command)
{
    t_command *command;
    char **args;

    while (commands->next)
        commands = commands->next;
    command = malloc(sizeof(t_command));
    if (!command)
        return;
    args = ft_split(current_command, ' ');
    if (!args)
        return;
    set_default_values(command);
    command->argv = args;
    commands->next = command;
}

t_command *get_commands(char *line)
{
    t_command *head;
    char **commands;
    int i;

    if (line[0] == '\0')
        return (NULL);
    commands = ft_split(line, '|');
    if (!commands)
        return (NULL);
    head = malloc(sizeof(t_command));
    if (!head)
        return (NULL);
    head->argv = ft_split(commands[0], ' ');
    head->next = NULL;
    head->pipes[0] = -1;
    head->pipes[1] = -1;
    i = 1;
    while (commands[i])
        get_cr_command(head, commands[i++]);
    return (head);
}