/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/12 06:22:18 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char *line)
{
	free(line);
	printf("exit\n");
	return (0);
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
		printf("%s%s%s@%s%s%s ", AMBER500, get_env("USER", minishell->env),
			RESET, VIOLET500, PROMPT, RESET);
		line = readline("→ ");
		if (line)
		{
			if (ft_strncmp(line, "exit", 4) == 0)
				return (ft_exit(line));
			minishell->commands = get_commands(line);
			execute_command(minishell);
			free_commands(minishell->commands);
			free(line);
		}
	}
	return (0);
}
