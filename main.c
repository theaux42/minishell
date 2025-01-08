/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/08 08:29:43 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_minishell *minishell)
{
	ft_freesplit(minishell->env);
	free_history(minishell->history);
	free(minishell->line);
	free(minishell);
	printf("exit\n");
	exit (0);
}

char	*clean_readline(char *prompt)
{
	char	*cleared;
	char	*line;

	rl_catch_signals = 0;
	if (!prompt)
		line = readline(DEFAULT_PROMPT);
	else
	{
		line = readline(prompt);
		free(prompt);
	}
	if (line)
	{
		cleared = ft_strtrim(line, " 	");
		free(line);
		if (cleared)
			return (cleared);
	}
	return (NULL);
}

void	main_loop(t_minishell *minishell)
{
	setup_signals();
	while (1)
	{
		minishell->line = clean_readline(nice_prompt(minishell->env));
		if (!minishell->line)
			break ;
		if (minishell->line && *minishell->line)
		{
			if (ft_strncmp(minishell->line, "exit ", 4) == 0)
				ft_exit(minishell);
			if (ft_strncmp(minishell->line, "history", 7) != 0)
				add_to_history(&minishell->history, minishell->line);
			minishell->commands = get_commands(minishell->line, minishell);
			execute_commands(minishell);
			// free_commands(minishell->commands);
			add_history(minishell->line);
			free(minishell->line);
		}
	}
	free_history(minishell->history);
}

int	main(int ac, char **av, char **env)
{
	t_minishell	*minishell;

	if (!isatty(STDIN_FILENO))
		return (printf("This is not a tty!\n"), 1);
	if (ac != 1 && ac != 3)
		return (printf("usage: ./minishell [-c command]\n"), 1);
	minishell = init_minishell(env);
	if (!minishell)
		return (1);
	if (ac == 1)
		main_loop(minishell);
	else if (ac == 3 && ft_strcmp(av[1], "-c") == 0)
	{
		minishell->commands = get_commands(av[2], minishell);
		execute_commands(minishell);
		free_commands(minishell->commands);
		return (minishell->status);
	}
	return (0);
}
