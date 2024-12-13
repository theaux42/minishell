/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:25:30 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/13 17:08:18 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_f_env(char *key)
{
    if (key && isdigit(key[0]))
        return (ft_substr(key, 1, ft_strlen(key) - 1));
    return (ft_strdup("tbabou"));
}

char *get_current_key(char *line, int *i)
{
    int start;
    int len;
    char *current_key;

    start = *i;
    while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
        (*i)++;
    len = *i - start;
    current_key = ft_substr(line, start, len);
    return (current_key);
}

char *expand_env_var(char *line, int *i, char *new_line)
{
    char *current_key;
    char *value;
    char *temp;

    (*i)++;
    current_key = get_current_key(line, i);
    if (current_key)
    {
        value = get_f_env(current_key);
        temp = ft_strjoin(new_line, value);
        free(new_line);
        new_line = temp;
        free(current_key);
        free(value);
    }
    else
        new_line = ft_addchar(new_line, '$');
    return new_line;
}

char *expand_line(char *line)
{
    char *new_line;
    int i;

    i = -1;
    new_line = ft_calloc(1, 1);
    while (line[++i])
    {
        if (line[i] == '$')
            new_line = expand_env_var(line, &i, new_line);
        else
            new_line = ft_addchar(new_line, line[i]);
    }
    return new_line;
}

bool need_expansion(char *value)
{
    int i;

    i = 0;
    if (value[i] == '\'' && value[ft_strlen(value) - 1] == '\'')
        return (false);
    while (value[i])
    {
        if (value[i] == '$')
            return (true);
        i++;
    }
    return (false);
}

void expand_commands(t_command *commands, t_minishell *minishell)
{
    t_command *current;
    
    current = commands;
    while (current)
    {
        if (need_expansion(current->tokens->value))
        {
            current->tokens->value = expand_line(current->tokens->value, minishell->env);
            if (!current->tokens->value)
                exit_error("expand_line");
        }
        current = current->next;
    }
}
