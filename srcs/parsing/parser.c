/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 22:41:56 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 14:47:36 by ededemog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_command	*init_new_command(char *token)
{
	t_command	*new_command;

	new_command = malloc(sizeof(t_command));
	if (!new_command)
		return (NULL);
	new_command->tokens = NULL;
	new_command->redirections = NULL;
	new_command->pipes[0] = -1;
	new_command->pipes[1] = -1;
	new_command->prev_pipe = -1;
	new_command->is_builtin = is_builtin(token);
	new_command->is_absolute = 0;
	new_command->is_last = 0;
	new_command->pid = 0;
	new_command->next = NULL;
	return (new_command);
}

t_token	*init_new_token(void)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = NULL;
	new_token->type = COMMAND;
	new_token->next = NULL;
	return (new_token);
}

void	add_tokens(t_token **head, char *token, int position)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->value = ft_strdup(token);
	new_token->type = get_tokens_type(token, position);
	new_token->next = NULL;
	if (*head == NULL)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
    printf("DEBUG: Added token '%s' of type %d\n", new_token->value, new_token->type); // Debug

}

char *ft_strcpy(char *dest, const char *src)
{
    size_t i;

    i = 0;
    if (!dest || !src)
        return (NULL);
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

static char *ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
    size_t len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
    char *result = malloc(len + 1);
    if (!result)
        return NULL;
    ft_strcpy(result, s1);
    ft_strcat(result, s2);
    ft_strcat(result, s3);
    return result;
}

void	parse_commands(t_command **head, char **commands)
{
	t_command	*current;
	t_command	*new_cmd;
	int			i;
	int			j;

	i = 0;
	j = 0;
	*head = init_new_command(commands[i]);
	current = *head;
	while (commands[i])
	{
		if (ft_strcmp(commands[i], "|") == 0)
		{
			j = -1;
			new_cmd = init_new_command(commands[i + 1]);
			current->next = new_cmd;
			current = new_cmd;
		}
		else if (get_redirection_type(commands[i]) != ARGUMENT)
		{
			add_tokens(&current->tokens, commands[i], j);
			if (!commands[i + 1])
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
				free_commands(*head);
				*head = NULL;
				return;
			}
			add_tokens(&current->tokens, commands[i + 1], -1);
			char *redir_line = ft_strjoin_three(commands[i], " ", commands[i + 1]);
			current->redirections = get_redirection(redir_line);
			free(redir_line);
			i++;
		}
		else
			add_tokens(&current->tokens, commands[i], j);
		i++;
		j++;
	}
}

t_command	*get_commands(char *line)
{
	t_command	*head;
	char		**commands;

	head = NULL;
	if (line[0] == '\0')
		return (NULL);
	commands = ft_ms_split(line);
	if (!commands)
		return (NULL);
	parse_commands(&head, commands);
	ft_freesplit(commands);
	if (!head)
		return (NULL);
	return (head);
}
