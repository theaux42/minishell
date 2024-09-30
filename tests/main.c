/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 22:45:36 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/30 03:54:55 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Commande pour compiler:  gcc main.c -L../libft -lft -L/opt/homebrew/opt/readline/lib -lreadline -g3 -o testing

#include "../minishell.h"

int ft_ms_isspace(char c)
{
    if (c == ' ' || c == '\t')
        return (1);
    return (0);
}

// Quand on arrive sur un quote on passe a 1
// Si on arrive sur un double quote et que is_in_arg == 1 on ignore le quote
// Si on arrive sur un quote et que is_in_arg == 1 on passe a 0
// Si on arrrive sur un double quote et que is_in_arg == 0 on passe a 2
// Si on arrive sur un quote et que is_in_arg == 2 on ignore
// Si on arrive sur un double quote et que is_in_arg == 2 on passe a 0

int quote_manager(char current, int is_in_arg)
{
    if (current == '\'')
    {
        if (is_in_arg == 0)
            is_in_arg = 1;  // Enter single quote state
        else if (is_in_arg == 1)
            is_in_arg = 0;  // Exit single quote state
    }
    else if (current == '"')
    {
        if (is_in_arg == 0)
            is_in_arg = 2;  // Enter double quote state
        else if (is_in_arg == 2)
            is_in_arg = 0;  // Exit double quote state
    }
    // State remains unchanged for other characters
    return is_in_arg;
}

int ft_prompt_length(char *line)
{
    int i = 0;
    int word_count = 0;
    int is_in_arg = 0;

    while (line[i])
    {
        while (line[i] && ft_ms_isspace(line[i]))
            i++;
        if (line[i])
        {
            word_count++;
            while (line[i] && (is_in_arg != 0 || !ft_ms_isspace(line[i])))
            {
                is_in_arg = quote_manager(line[i], is_in_arg);
                i++;
            }
        }
    }
    return word_count;
}

int inner_split(char **split, char *line)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int is_in_arg = 0;

    while (line[i])
    {
        while (line[i] && ft_ms_isspace(line[i]))
            i++;
        if (line[i] && !ft_ms_isspace(line[i]))
        {
            j = i;
            is_in_arg = 0;
            while (line[i] && (is_in_arg != 0 || !ft_ms_isspace(line[i])))
                is_in_arg = quote_manager(line[i++], is_in_arg);   
            split[k] = malloc(i - j + 1);
            if (!split[k])
                return -1;
            strncpy(split[k], &line[j], i - j);
            split[k][i - j] = '\0';
            k++;
        }
    }
    return is_in_arg;
}

char **ft_ms_split(char *line)
{
    char **args;
    int result;

    args = malloc(sizeof(char *) * (ft_prompt_length(line) + 10));
    if (!args)
        return NULL;
    result = inner_split(args, line);
    if (result == -1)
    {
        free(args);
        return NULL;
    }
    if (result != 0)
    {
        ft_freesplit(args);
        printf("%sInvalid prompt%s\n", RED800, RESET);
        return NULL;
    }
    args[ft_prompt_length(line)] = NULL;
    return args;
}

void ft_print_prompt(char **commands)
{
    int i = 0;
    while (commands[i])
    {
        printf("Token %d: %s\n", i, commands[i]);
        i++;
    }
}

void parse_commands(char *line)
{
    char **commands;
    commands = ft_ms_split(line);
    if (!commands)
        return;
    ft_print_prompt(commands);
}

int main(void)
{
    char *line;
    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
            return (0);
        if (ft_strncmp(line, "exit", 4) == 0)
            exit(1);
        parse_commands(line);
        free(line);
    }
    return (0);
}

