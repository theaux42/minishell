#include "minishell.h"


int ft_chrinstr(char *str, char c);
int define_type(char *str);
void handle_double_quotes(char **str, int *i, t_parser_state *state);
void handle_single_quotes(char **str, int *i, t_parser_state *state);
void handle_quotes(char **str, int *i, t_parser_state *state);
void handle_command(char **str, int *i, int *is_next_command, t_prompt **next);

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

void interpreter(t_prompt *prompt)
{
    int i;
    int in_double_str;
    int in_single_str;
    t_prompt *next;
    char *args;

    i = 0;
    in_double_str = 0;
    in_single_str = 0;
    next = prompt;
    while (next->next)
    {
        if (ft_chrinstr(next->value, '"') && !in_double_str && !in_single_str)
        {
            in_double_str = 1;
            args = ft_strdup(next->value);
        }
        else if (ft_chrinstr(next->value, '\'') && !in_single_str && !in_double_str)
        {
            in_single_str = 1;
            args = ft_strdup(next->value);
        }
        else if (!ft_chrinstr(next->value, '"') && in_double_str)
        {
            args = ft_strjoin(args, " ");
            args = ft_strjoin(args, next->value);
        }
        else if (!ft_chrinstr(next->value, '\'') && in_single_str)
        {
            args = ft_strjoin(args, " ");
            args = ft_strjoin(args, next->value);
        }
        else if (ft_chrinstr(next->value, '"') && in_double_str)
        {
            in_double_str = 0;
            args = ft_strjoin(args, " ");
            args = ft_strjoin(args, next->value);
            printf("args = %s\n", args);
            free(args);
        }
        else if (ft_chrinstr(next->value, '\'') && in_single_str)
        {
            in_single_str = 0;
            args = ft_strjoin(args, " ");
            args = ft_strjoin(args, next->value);
            printf("args = %s\n", args);
            free(args);
        }

        next = next->next;
    }
}

void ft_print_prompt(t_prompt *prompt)
{
    t_prompt *next;
    int i;

    i = 0;
    next = prompt;
    while (next->next)
    {
        printf("[DEBUG] => %d - type = %s - value = %s\n", i++, type_to_str(next->type), next->value);
        next = next->next;
    }
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

void handle_command(char **str, int *i, int *is_next_command, t_prompt **next)
{
    if (*is_next_command)
    {
        (*next)->type = COMMAND;
        *is_next_command = 0;
    }
    else
        (*next)->type = define_type(str[*i]);
    if ((*next)->type == SEMICOLON || (*next)->type == PIPE || (*next)->type == REDIRECTION)
        *is_next_command = 1;
    (*next)->value = str[*i];
    (*next)->next = malloc(sizeof(t_prompt));
    *next = (*next)->next;
}

void handle_double_quotes(char **str, int *i, t_parser_state *state)
{
    if (str[*i][0] == '"' && str[*i][strlen(str[*i]) - 1] == '"')
    {
        state->next->value = ft_strtrim(str[*i], "\"");
        state->next->next = malloc(sizeof(t_prompt));
        state->next = state->next->next;
    }
    else
    {
        state->in_double_str = 1;
        state->args = ft_strdup(str[*i]);
    }
}

void handle_single_quotes(char **str, int *i, t_parser_state *state)
{
    if (str[*i][0] == '\'' && str[*i][strlen(str[*i]) - 1] == '\'')
    {
        state->next->value = ft_strtrim(str[*i], "'");
        state->next->next = malloc(sizeof(t_prompt));
        state->next = state->next->next;
    }
    else
    {
        state->in_single_str = 1;
        state->args = ft_strdup(str[*i]);
    }
}

void handle_quotes(char **str, int *i, t_parser_state *state)
{
    if (state->in_double_str || state->in_single_str)
    {
        state->args = ft_strjoin(state->args, " ");
        state->args = ft_strjoin(state->args, str[*i]);
        if ((state->in_double_str && ft_chrinstr(str[*i], '"')) || (state->in_single_str && ft_chrinstr(str[*i], '\'')))
        {
            if (state->in_double_str)
                state->in_double_str = 0;
            if (state->in_single_str)
                state->in_single_str = 0;
            state->args = ft_strtrim(state->args, "\"");
            state->args = ft_strtrim(state->args, "'");
            state->next->value = state->args;
            state->next->type = ARGUMENT; // Ensure the type is set correctly
            state->next->next = malloc(sizeof(t_prompt));
            state->next = state->next->next;
            state->args = NULL;
        }
    }
    else
    {
        if (ft_chrinstr(str[*i], '"') && !state->in_single_str)
            handle_double_quotes(str, i, state);
        else if (ft_chrinstr(str[*i], '\'') && !state->in_double_str)
            handle_single_quotes(str, i, state);
    }
}

t_prompt *parser(char **str, t_prompt *head)
{
    t_prompt *next;
    int is_next_command;
    int i;
    t_parser_state state;

    i = 1;
    is_next_command = 0;
    state.in_double_str = 0;
    state.in_single_str = 0;
    state.args = NULL;
    next = malloc(sizeof(t_prompt));
    head->type = COMMAND;
    head->value = str[0];
    head->next = next;
    state.next = next;
    while (str[i])
    {
        handle_quotes(str, &i, &state);
        if (!state.in_double_str && !state.in_single_str)
            handle_command(str, &i, &is_next_command, &state.next);
        i++;
    }
    state.next->next = NULL;
    return (head);
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
            prompt = parser(args, prompt);
            // interpreter(prompt);
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