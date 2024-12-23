/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:12:24 by ededemog          #+#    #+#             */
/*   Updated: 2024/12/16 10:41:41 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_to_history(t_history **history, char *command)
{
	t_history	*new_entry;
	t_history	*current;

	new_entry = malloc(sizeof(t_history));
	if (!new_entry)
		return ;
	new_entry->command = ft_strdup(command);
	new_entry->next = NULL;
	if (!*history)
		*history = new_entry;
	else
	{
		current = *history;
		while (current->next)
			current = current->next;
		current->next = new_entry;
	}
}

void	print_history(t_history *history)
{
	int	index;

	index = 1;
	while (history)
	{
		printf("%d %s\n", index++, history->command);
		history = history->next;
	}
}

void	free_history(t_history *history)
{
	t_history	*current;

	while (history)
	{
		current = history;
		history = history->next;
		free(current->command);
		free(current);
	}
}
