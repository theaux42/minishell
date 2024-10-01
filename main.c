/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/01 14:10:43 by tbabou           ###   ########.fr       */
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

int main(void)
{
    t_command  *command;
    char       *line;
    while (1)
    {
        line = readline(PROMPT);
        if (line)
        {
            if (ft_strncmp(line, "exit", 4) == 0)
                return (ft_exit(line));
        command = get_commands(line);
         execute_command(command);
        }
    }
    return 0;
}