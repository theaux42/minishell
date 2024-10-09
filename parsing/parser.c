/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 22:41:56 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/08 16:46:33 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	make_tokens(t_token **tokens_ptr, char **args, int i)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;

	head = NULL;
	current = NULL;
	while (args[i])
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			return (0);
		new_token->value = ft_token_value(args[i]);
		if (!new_token->value)
			return (0);
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
	return (1);
}

t_token	*set_tokens_type(char *command)
{
	char	**args;
	t_token	*tokens;
	int		result;

	args = ft_ms_split(command);
	if (!args)
		return (NULL);
	tokens = NULL;
	result = make_tokens(&tokens, args, 0);
	ft_freesplit(args);
	if (!result)
		return (NULL);
	return (tokens);
}

void	get_cr_command(t_command **cmd_ptr, char **commands)
{
	t_command	*head;
	t_command	*current;
	int			i;
	t_command	*new_cmd;

	head = NULL;
	current = NULL;
	i = 0;
	while (commands[i])
	{
		new_cmd = malloc(sizeof(t_command));
		if (!new_cmd)
			return ;
		new_cmd->tokens = set_tokens_type(commands[i]);
		if (!new_cmd->tokens)
			return ;
		new_cmd->next = NULL;
		if (!head)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		i++;
	}
	*cmd_ptr = head;
}

t_command	*get_commands(char *line)
{
	t_command	*head;
	char		**commands;

	head = NULL;
	if (line[0] == '\0')
		return (NULL);
	commands = ft_split(line, '|');
	if (!commands)
		return (NULL);
	get_cr_command(&head, commands);
	ft_freesplit(commands);
	print_commands(head);
	return (head);
}
