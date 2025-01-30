/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 08:39:30 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/29 23:01:25 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	validate_commands(t_command *commands, t_minishell *minishell)
{
    t_command	*cur;

    cur = commands;
    while (cur)
    {
        if (!cur->tokens && !cur->redirections)
        {
            minishell->status = 2;
            return (ft_dprintf(2, ERR_EMPTY_CMD), false);
        }
        if (cur->tokens && cur->tokens[0].value == NULL)
        {
            minishell->status = 2;
            return (ft_dprintf(2, "minishell: malloc error\n"), false);
        }
        cur = cur->next;
    }
    return (true);
}

void	error_message(char *title, char *message)
{
	perror(title);
	ft_dprintf(2, "%s\n", message);
}

void	exit_parent(char *msg, t_minishell *minishell, bool is_error)
{
	int	exit_status;

	exit_status = EXIT_FAILURE;
	if (msg != NULL)
	{
		if (is_error)
			ft_dprintf(2, "%s", msg);
		else
			ft_printf("%s", msg);
	}
	if (minishell->commands)
		free_commands(minishell->commands);
	if (!is_error)
		exit_status = minishell->status;
	ft_freesplit(minishell->env);
	free(minishell->line);
	free(minishell);
	rl_clear_history();
	exit(exit_status);
}

void	exit_child(char *msg, t_minishell *minishell, char *cmd, char **argv)
{
	if (msg != NULL)
		ft_dprintf(2, "%s", msg);
	if ((cmd && cmd != NULL) && is_builtin(cmd))
		free(cmd);
	free_commands(minishell->commands);
	rl_clear_history();
	ft_freesplit(minishell->env);
	free(minishell->line);
	if (minishell->fds[STDIN_FILENO] != -1)
		close(minishell->fds[STDIN_FILENO]);
	if (minishell->fds[STDOUT_FILENO] != -1)
		close(minishell->fds[STDOUT_FILENO]);
	free(minishell);
	free(argv);
	exit(EXIT_FAILURE);
}

bool	ft_isfolder(char *path)
{
	struct stat	stats;

	if (stat(path, &stats) == -1)
		return (false);
	return (S_ISDIR(stats.st_mode));
}