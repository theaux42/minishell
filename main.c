/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/24 16:33:43 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type get_redirection_type(char *str);
int command_length(t_prompt *prompt, int position);

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

    // Si le dernier token était un PIPE, le suivant doit être une COMMAND
    if (last_type == PIPE)
    {
        last_type = COMMAND;
        return (COMMAND);
    }

    // Vérifier si le token est une option (ex: -l)
    if (is_option(str)) // Assurez-vous que cette fonction est correctement implémentée
        return (OPTION);

    // Vérifier si le token est un PIPE
    if (strcmp(str, "|") == 0)
    {
        last_type = PIPE;
        return (PIPE);
    }

    // Vérifier si le token est une redirection
    t_token_type redir_type = get_redirection_type(str);
    if (redir_type != ARGUMENT)
        return (redir_type);

    // Sinon, c'est un ARGUMENT ou une COMMAND
    // Vous pouvez affiner cette logique selon vos besoins
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

void ft_print_prompt(t_prompt *prompt)
{
    t_prompt *next;
    int i;

    i = 0;
    next = prompt;
    while (next)
    {
        printf("[DEBUG] => %d - type = %d - value = %s\n", i++, next->type, next->value);
        next = next->next;
    }

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
        printf("]\n");
        next = next->next;
    }
}

t_prompt *init_prompt(void)
{
    t_prompt *command;

    command = malloc(sizeof(t_prompt));
    command->value = "echo";
    command->type = COMMAND;
    command->next = NULL;
    
    add_token(command, "-n");
    add_token(command, "Hello World");
    add_token(command, "Hello World");
    add_token(command, "Hello World");
    add_token(command, "Hello World");
    add_token(command, "|");
    add_token(command, "grep");
    add_token(command, "file.txt");
    add_token(command, "|");
    add_token(command, "cat");
    add_token(command, "-e");
    add_token(command, "-e");
    add_token(command, "file.txt");
    
    return (command);
}

void ft_print_split(char **args, int oui)
{
    int i = 0;
    printf("command n%i ->", oui);
    while (args[i])
    {
        printf("%s ", args[i]);
        i++;
    }
    printf("\n");
}

void get_cr_command(t_command *commands, char *current_command)
{
    t_command *command;
    char **args;
    // int i;

    // i = 0;
    while (commands->next)
        commands = commands->next;
    command = malloc(sizeof(t_command));
    args = ft_split(current_command, ' ');
    commands->next = command;
    command->argv = args;
    command->next = NULL;
    command->redirections = NULL;
}

t_command *get_commands(char *line)
{
    t_command *head;
    char **commands;
    int i;

    i = 0;
    commands = ft_split(line, '|');
    head = malloc(sizeof(t_command));
    head->argv = ft_split(commands[i], ' ');
    head->next = NULL;
    while (commands[++i])
    {
        get_cr_command(head, commands[i]);
    }
    return (head);
}

int main(void)
{
    t_command  *command;
    char       *line;
    while (1)
    {
        line = readline(PROMPT);
        if (ft_strncmp(line, "exit", 4) == 0)
            return (ft_exit(line));
        command = get_commands(line);
        ft_print_commands(command);
    }
    return 0;
}