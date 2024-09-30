/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/29 22:41:47 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type get_redirection_type(char *str);

int ft_exit(char *line)
{
    free(line);
    printf("exit\n");
    return (0);
}

int is_option(char *str)
{
    if (str[0] == '-')
        return (1);
    return (0);
}

int ft_chrinstr(char *str, char c)
{
    while (str && *str)
    {
        if (*str == c)
            return (1);
        str++;
    }
    return (0);
}

int is_string(char *str)
{
    if (ft_chrinstr(str, '"'))
        return (1);
    if (ft_chrinstr(str, '\''))
        return (2);
    return (0);
}

t_token_type define_type(char *str)
{
    static t_token_type last_type = COMMAND;
    if (last_type == PIPE)
    {
        last_type = COMMAND;
        return (COMMAND);
    }
    if (is_option(str))
        return (OPTION);
    if (strcmp(str, "|") == 0)
    {
        last_type = PIPE;
        return (PIPE);
    }
    t_token_type redir_type = get_redirection_type(str);
    if (redir_type != ARGUMENT)
        return (redir_type);
    return (ARGUMENT);
}

t_token_type get_redirection_type(char *str)
{
    if (strncmp(str, ">>", 2) == 0)
        return (REDIRECTION_APPEND);
    if (strncmp(str, ">", 1) == 0)
        return (REDIRECTION_OUTPUT);
    if (strncmp(str, "<", 1) == 0)
        return (REDIRECTION_INPUT);
    if (strncmp(str, "<<", 2) == 0)
        return (REDIRECTION_HEREDOC);
    // Ajouter d'autres types de redirections si nécessaire
    return (ARGUMENT); // Par défaut
}

void add_token(t_prompt *prompt, char *str)
{
    t_prompt *next;
    t_prompt *new;

    next = prompt;
    while (next->next)
        next = next->next;
    new = malloc(sizeof(t_prompt));
    new->value = str;
    new->type = define_type(str);
    new->next = NULL;
    next->next = new;
}

void ft_print_commands(t_command *command)
{
    t_command *next;
    int i;
    int j;

    i = 0;
    next = command;
    while (next)
    {
        j = 0;
        printf("[DEBUG] => %d - argv = [", i++);
        while (next->argv[j])
            printf("%s, ", next->argv[j++]);
        printf("] - LAST \n");
        next = next->next;
    }
}

int main(void)
{
    t_command  *command;
    char       *line;
    while (1)
    {
        line = readline(PROMPT);
        if (line)
        {
        command = get_commands(line);
        execute_command(command);
        }
    }
    return 0;
}