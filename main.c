/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/13 03:35:14 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char *line, t_minishell *minishell)
{
	ft_freesplit(minishell->env);
	free(minishell);
	free(line);
	printf("exit\n");
	return (0);
}

char	*clean_readline(void)
{
	char	*cleared;
	char	*line;

	line = readline("→ ");
	if (line)
	{
		cleared = ft_strtrim(line, " 	");
		if (cleared)
		{
			free(line);
			return (cleared);
		}
		else
		{
			free(line);
			return (NULL);
		}
	}
	return (NULL);
}

int	main(int ac, char **av, char **env)
{
	t_minishell	*minishell;
	char		*line;
	char		*uname;

	(void)ac;
	(void)av;
	minishell = init_minishell(env);
	while (1)
	{
		uname = get_env("USER", minishell->env);
		printf("%s%s%s@%s%s%s ", AMBER500, uname, RESET, VIOLET500, PROMPT,
			RESET);
		line = clean_readline();
		if (line && *line)
		{
			if (ft_strncmp(line, "exit ", 4) == 0)
				return (ft_exit(line, minishell));
			minishell->commands = get_commands(line, minishell);
			execute_commands(minishell);
			free_commands(minishell->commands);
			free(line);
		}
	}
	return (0);
}
