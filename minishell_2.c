#include "minishell.h"

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

char *type_to_str(int type)
{
    if (type == COMMAND)
        return ("COMMAND");
    if (type == ARGUMENT)
        return ("ARGUMENT");
    if (type == OPTIONS)
        return ("OPTIONS");
    if (type == REDIRECTION)
        return ("REDIRECTION");
    if (type == PIPE)
        return ("PIPE");
    if (type == SEMICOLON)
        return ("SEMICOLON");
    return ("UNKNOWN");
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

int define_type(char *str)
{
    if (is_option(str))
        return (OPTIONS);
    if (str[0] == '|')
        return (PIPE);
    if (str[0] == ';')
        return (SEMICOLON);
    if (str[0] == '>')
        return (REDIRECTION);
    return (ARGUMENT);
}

void add_token(t_prompt *prompt, char *str)
{
    static int last_type = 0;
    t_prompt *next;
    t_prompt *new;

    next = prompt;
    while (next->next)
        next = next->next;
    new = malloc(sizeof(t_prompt));
    new->value = str;
    if (last_type == PIPE)
    {
        new->type = COMMAND;
        last_type = 0;
    }
    else
        new->type = define_type(str);
    last_type = new->type;
    new->next = NULL;
    next->next = new;
}

t_prompt *first_parsing(char **args)
{
    t_prompt *prompt;
    t_prompt *new;
    int       i;

    i = 1;
    prompt = malloc(sizeof(t_prompt));
    prompt->value = args[0];
    prompt->type = COMMAND;
    prompt->next = NULL;
    while (args[i])
        add_token(prompt, args[i++]);
        i++;
    return (prompt);
}
void ft_print_prompt(t_prompt *prompt)
{
    t_prompt *next;
    int i;

    i = 0;
    next = prompt;
    while (next)
    {
        printf("[DEBUG] => %d - type = %s - value = %s\n", i++, type_to_str(next->type), next->value);
        next = next->next;
    }

}

t_prompt *get_prompt_at_index(t_prompt *prompt, int i)
{
    int count = 0;
    t_prompt *current = prompt;

    while (current != NULL && count < i)
    {
        current = current->next;
        count++;
    }

    return current;
}

void print_split(char **args)
{
    int i = 0;
    printf("args\n");
    while (args[i])
    {
        printf("%s\n", args[i]);
        i++;
    }
    printf("end args\n");
}

void pipe_in_str(char *str, t_prompt *prompt, int id)
{
    int i = 0;
    char **args;
    t_prompt *new;
    t_prompt *next;

    args = ft_split(str, '|');
    while (args[i])
    {
        new = malloc(sizeof(t_prompt));
        new->value = args[i];
        new->type = define_type(args[i]);
        new->next = NULL;
        next = get_prompt_at_index(prompt, id);
        while (next->next)
            next = next->next;
        next->next = new;
        i++;
    }
}

t_prompt *pipe_parsing(t_prompt *prompt)
{
    t_prompt *next;
    int       i;

    i = 0;
    next = prompt;
    while (next)
    {
        if (ft_chrinstr(next->value, '|'))
            pipe_in_str(next->value, prompt, i);
        next = next->next;
        i++;
    }
    return (prompt);
}

t_prompt *parser(char **args)
{
    t_prompt *prompt;

    prompt = first_parsing(args);
    prompt = pipe_parsing(prompt);
    return (prompt);
}

int main()
{
    char *line;
    char **args;
    t_prompt *prompt;
    while (1)
    {
        line = readline(PROMPT);
        if (strcmp(line, "exit") == 0)
        {
            return (ft_exit(line));
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            args = ft_split(line, ' ');
            prompt = malloc(sizeof(t_prompt));
            prompt = parser(args);
            ft_print_prompt(prompt);
            execvp(args[0], args);
            perror("exec");
            ft_freesplit(args);
            exit(1);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}