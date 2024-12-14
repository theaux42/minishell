/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:25:30 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/14 05:50:00 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_env_var(char *line, int *i, char *new_line, t_minishell *minishell)
{
    char *current_key;
    char *value;
    char *temp;

    (*i)++;
    current_key = get_current_key(line, i);
    if (current_key)
    {
        value = get_env_var(current_key, minishell);
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

char *expand_line_2(char *line, t_minishell *minishell)
{
    char *new_line;
    int i;

    i = 0;
    new_line = ft_calloc(1, 1);
    if (!new_line)
        return (NULL);
    while (line[i] && line[i] != '\0')
        if (line[i] == '$')
        {
            new_line = expand_env_var(line, &i, new_line, minishell);
            if (!new_line)
                return (NULL);
        }
        else
        {
            new_line = ft_addchar(new_line, line[i++]);
            if (!new_line)
                return (NULL);
        }
    return (new_line);
}

void expand_tokens(t_token *tokens, t_minishell *minishell)
{
    t_token *current_token = tokens;
    bool need_expand;

    need_expand = false;
    while (current_token)
    {
        if (need_expansion(current_token->value))
            need_expand = true;
        else
            need_expand = false;
        if (check_missused_quotes(current_token->value))
            exit_error("check_missused_quotes");
        current_token->value = process_quote(current_token->value);
        if (!current_token->value)
            exit_error("remove_quotes");
        if (need_expand)
        {
            current_token->value = expand_line_2(current_token->value, minishell);
            if (!current_token->value)
                exit_error("expand_line_2");
        }
        current_token = current_token->next;
    }
}

bool check_commands(t_command *commands)
{
    t_command *current_command;
    t_token *current_token;

    current_command = commands;
    while (current_command)
    {
        current_token = current_command->tokens;
        while (current_token)
        {
            if (check_missused_quotes(current_token->value))
                return (false);
            current_token = current_token->next;
        }
        current_command = current_command->next;
    }
    return (true);
}

bool expand_commands(t_command *commands, t_minishell *minishell)
{
    t_command *current = commands;

    if (!check_commands(commands))
    {
        printf("[%sError%s]: missused quotes\n", RED500, RESET);
        return (false);
    }
    while (current)
    {
        expand_tokens(current->tokens, minishell);
        current = current->next;
    }
    return (true);
}
