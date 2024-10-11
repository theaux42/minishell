/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/11 21:21:05 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char *line)
{
	free(line);
	printf("exit\n");
	return (0);
}

int	main(void)
{
	t_command	*command;
	char		*line;

	while (1)
	{
		printf("%s%s%s ", VIOLET500, PROMPT, RESET);
		line = readline("→ ");
		if (line)
		{
			if (ft_strncmp(line, "exit", 4) == 0)
				return (ft_exit(line));
			command = get_commands(line);
			execute_command(command);
			free_commands(command);
			free(line);
		}
	}
	return (0);
}
