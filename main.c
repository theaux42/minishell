/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:14:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/13 18:54:29 by ededemog         ###   ########.fr       */
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

char    *clean_readline(void)
{
    char    *cleared;
    char    *line;

    line = readline("→ ");
    if (line && *line)
    {
        cleared = ft_strtrim(line, "     ");
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

char	*prompt_handler(t_minishell *minishell)
{
	char	*uname;
	char	*strings[9];
	char	*prompt;

	uname = get_env("USER", minishell->env);
	if (!uname)
		uname = ft_strdup("unknown");
	else
		uname = ft_strdup(uname);
	strings[0] = AMBER500;
	strings[1] = uname;
	strings[2] = RESET;
	strings[3] = "@";
	strings[4] = VIOLET500;
	strings[5] = PROMPT;
	strings[6] = RESET;
	strings[7] = " ";
	strings[8] = "→ ";

	prompt = ft_strjoin_multi(9, strings);
	free(uname);

	return (prompt);
}

int	main(int ac, char **av, char **env)
{
	t_minishell	*minishell;
	char		*line;
	char		*prompt;

	(void)ac;
	(void)av;
	minishell = init_minishell(env);
	setup_signals(); 
	while (1)
	{
		prompt = prompt_handler(minishell);
		line = readline(prompt);
		free(prompt);
		if (!line)
		{
			printf("exit\n");
			free_history(minishell->history);
			ft_freesplit(minishell->env);
			free(minishell);
			exit(0);
		}
		if (line && *line)
		{
			if (ft_strncmp(line, "exit ", 4) == 0)
				return (ft_exit(line, minishell));
            if (ft_strncmp(line, "history", 7) != 0)
                add_to_history(&minishell->history, line);
			minishell->commands = get_commands(line);
			execute_commands(minishell);
			free_commands(minishell->commands);
			add_history(line);

		}
		free(line);
	}
	free_history(minishell->history);
	return (0);
}
