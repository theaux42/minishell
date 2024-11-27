#include "minishell.h"

void add_to_history(t_history **history, char *command)
{
    t_history *new_entry = malloc(sizeof(t_history));
    if (!new_entry)
        return;
    new_entry->command = ft_strdup(command);
    new_entry->next = NULL;

    if (!*history)
        *history = new_entry;
    else
    {
        t_history *current = *history;
        while (current->next)
            current = current->next;
        current->next = new_entry;
    }
}

void print_history(t_history *history)
{
    int index = 1;
    while (history)
    {
        printf("%d %s\n", index++, history->command);
        history = history->next;
    }
}

void free_history(t_history *history)
{
    t_history *current;
    while (history)
    {
        current = history;
        history = history->next;
        free(current->command);
        free(current);
    }
}