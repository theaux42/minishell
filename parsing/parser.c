/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 22:41:56 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/01 14:10:56 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *type_to_str(t_token_type type)
{
    if (type == COMMAND)
        return "COMMAND";
    if (type == PIPE)
        return "PIPE";
    if (type == REDIRECTION_INPUT)
        return "REDIRECTION_INPUT";
    if (type == REDIRECTION_OUTPUT)
        return "REDIRECTION_OUTPUT";
    if (type == REDIRECTION_APPEND)
        return "REDIRECTION_APPEND";
    if (type == REDIRECTION_HEREDOC)
        return "REDIRECTION_HEREDOC";
    if (type == ARGUMENT)
        return "ARGUMENT";
    return "UNKNOWN";
}

void make_tokens(t_token **tokens_ptr, char **args)
{
    t_token *head = NULL;
    t_token *current = NULL;
    int i = 0;

    while (args[i])
    {
        t_token *new_token = malloc(sizeof(t_token));
        if (!new_token)
            return;
        new_token->value = ft_token_value(args[i]);
        if (!new_token->value)
            return;
        new_token->type = get_tokens_type(args[i], i);
        new_token->next = NULL;
        if (!head)
            head = new_token;
        else
            current->next = new_token;
        current = new_token;
        i++;
    }
    *tokens_ptr = head;
}

t_token *set_tokens_type(char **args)
{
    t_token *tokens = NULL;
    make_tokens(&tokens, args);
    return tokens;
}

void get_cr_command(t_command **cmd_ptr, char **commands)
{
    t_command *head = NULL;
    t_command *current = NULL;
    int i = 0;

    while (commands[i])
    {
        char **args = ft_ms_split(commands[i]);
        if (!args)
            return;
        t_command *new_cmd = malloc(sizeof(t_command));
        if (!new_cmd)
            return;
        new_cmd->tokens = set_tokens_type(args);
        new_cmd->next = NULL;
        if (!head)
            head = new_cmd;
        else
            current->next = new_cmd;
        current = new_cmd;
        ft_freesplit(args);
        i++;
    }
    *cmd_ptr = head;
}

void ft_print_prompt(t_command *command)
{
    t_token *tokens;
    int i;
    int j = 0;

    while (command)
    {
        tokens = command->tokens;
        i = 0;
        printf("Command %d:\n", j++);
        while (tokens)
        {
            printf("Token %d: %s - %s\n", i++, tokens->value,
            type_to_str(tokens->type));
            tokens = tokens->next;
        }
        command = command->next;
    }
}

t_command *get_commands(char *line)
{
    t_command *head = NULL;
    char **commands;

    if (line[0] == '\0')
        return (NULL);
    commands = ft_split(line, '|');
    if (!commands)
        return (NULL);
    get_cr_command(&head, commands);
    ft_freesplit(commands);
    // ft_print_prompt(head);
    return head;
}