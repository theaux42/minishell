/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 06:15:02 by tbabou           ###   ########.fr       */
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

char	*clean_readline(char *prompt)
{
	char	*cleared;
	char	*line;

	if (!prompt)
		line = readline("no_prompt :( → ");
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

int	main(int ac, char **av, char **env)
{
	t_minishell	*minishell;
	char		*line;

	(void)ac;
	(void)av;
	minishell = init_minishell(env);
	while (1)
	{
		line = clean_readline(nice_prompt(minishell->env));
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
