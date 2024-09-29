/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/29 14:57:32 by tbabou           ###   ########.fr       */
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

char **split_args(const char *line)
{
    char **args = NULL;
    int count = 0;
    int i = 0;
    int j = 0;
    char quote = '\0';
    char buffer[1024];

    while (line[i])
    {
        j = 0;
        // Sauter les espaces initiaux
        while (line[i] == ' ')
            i++;
        if (line[i] == '\0')
            break;
        // Parcourir l'argument
        while (line[i] && (quote || (line[i] != ' ')))
        {
            if ((line[i] == '\'' || line[i] == '"') && !quote)
                quote = line[i++];
            else if (line[i] == quote)
            {
                quote = '\0';
                i++;
            }
            else
                buffer[j++] = line[i++];
        }
        buffer[j] = '\0';
        args = realloc(args, sizeof(char *) * (count + 2));
        args[count++] = strdup(buffer);
    }
    if (args)
        args[count] = NULL;
    return args;
}

void get_cr_command(t_command *commands, char *current_command)
{
    t_command *command;
    char **args;
    int i;

    while (commands->next)
        commands = commands->next;
    command = malloc(sizeof(t_command));
    if (!command)
        return;
    args = ft_split(current_command, ' ');
    command->argv = args;
    command->next = NULL;
    command->redirections = NULL;
    command->pipes[0] = -1;
    command->pipes[1] = -1;
    commands->next = command;

    // **Ajout de printf pour déboguer**
    printf("Nouvelle commande créée :\n");
    i = 0;
    while (args[i])
    {
        printf("Arg[%d]: %s\n", i, args[i]);
        i++;
    }
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
    head->redirections = NULL;
    head->pipes[0] = -1;
    head->pipes[1] = -1;

    // **Ajout de printf pour déboguer**
    printf("Commande initiale :\n");
    i = 0;
    while (head->argv[i])
    {
        printf("Arg[%d]: %s\n", i, head->argv[i]);
        i++;
    }

    i = 1;
    while (commands[i])
    {
        get_cr_command(head, commands[i]);
        i++;
    }
    // Libérer `commands` si nécessaire
    return (head);
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