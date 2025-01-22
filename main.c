/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/22 14:37:13 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			exit_parent("exit\n", minishell, false);
		if (minishell->line && *minishell->line)
		{
			minishell->commands = get_commands(minishell->line, minishell);
			if (DEBUG_MODE)
				print_commands(minishell->commands);
			execute_commands(minishell);
			minishell->commands = NULL;
			add_history(minishell->line);
		}
		free(minishell->line);
	}
}

int	main(int ac, char **av, char **env)
{
	t_minishell	*minishell;

	(void)ac;
	(void)av;
	if (!isatty(STDIN_FILENO))
		return (ft_dprintf(2, ERR_NOT_A_TTY), 1);
	if (DEBUG_MODE)
		ft_dprintf(2, DEBUG_MSG);
	minishell = init_minishell(env);
	if (!minishell)
		return (1);
	main_loop(minishell);
	return (0);
}
