/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/29 00:49:19 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*clean_readline(void)
{
    char	*cleared;
    char	*line;

    rl_catch_signals = 0;
    line = readline(DEFAULT_PROMPT);
    if (!line)
    {
        ft_dprintf(2, "Error: readline returned NULL\n");
        return (NULL);
    }
    cleared = ft_strtrim(line, " \t\n");
    free(line);
    if (!cleared)
    {
        ft_dprintf(2, "Error: ft_strtrim returned NULL\n");
        ft_dprintf(2, ERR_MALLOC);
        return (NULL);
    }
    return (cleared);
}

volatile sig_atomic_t	g_signal = 0;

void	main_loop(t_minishell *minishell)
{
	while (1)
	{
		ft_signal();
		minishell->line = clean_readline();
		if (!minishell->line)
		{
			if (g_signal == 0) // EOF/Ctrl-D
				exit_parent("exit\n", minishell, false);
			minishell->status = g_signal;
			g_signal = 0;
			continue ;
		}
		if (*minishell->line)
		{
			minishell->commands = get_commands(minishell->line, minishell);
			if (!minishell->commands)
			{
				free(minishell->line);
				continue ;
			}
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
	// if (!isatty(STDIN_FILENO))
	// 	return (ft_dprintf(2, ERR_NOT_A_TTY), 1);
	if (DEBUG_MODE)
		ft_dprintf(2, DEBUG_MSG);
	minishell = init_minishell(env);
	if (!minishell)
		return (1);
	main_loop(minishell);
	return (0);
}
