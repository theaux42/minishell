/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:19:28 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/28 13:42:55 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	overflows(char *str)
{
	int	i;
	int	sign;
	int	len;

	i = 0;
	sign = 1;
	len = ft_strlen(str);
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	if (len > 10)
		return (true);
	if (len == 10 && sign == 1 && ft_strncmp(str, "2147483647", 10) > 0)
		return (true);
	if (len == 10 && sign == -1 && ft_strncmp(str, "2147483648", 10) > 0)
		return (true);
	return (false);
}

static bool	init_exit(t_token *token, t_minishell *minishell)
{
	int	status;

	status = minishell->status;
	if (token == NULL || token->value == NULL)
		return (-1);
	if (!ft_isdigits_str(token->value) || overflows(token->value))
	{
		minishell->status = 2;
		return (ft_dprintf(2, ERR_EXIT_NUM, token->value), true);
	}
	if (ft_cmdlen(token) > 1)
		return (ft_dprintf(2, ERR_TOO_MANY_ARGS, "exit"), false);
	minishell->status = (unsigned int)ft_atoi(token->value);
	if (minishell->status == -1)
		minishell->status = status;
	return (true);
}

int	ft_exit(t_token *token, t_minishell *minishell, bool msg)
{
	int		status;
	bool	do_exit;

	if (msg)
		ft_printf("exit\n");
	do_exit = init_exit(token, minishell);
	status = minishell->status;
	if (!msg)
		return (status);
	if (!do_exit)
		return (1);
	free_commands(minishell->commands);
	ft_free_builtins(minishell);
	exit(status);
}
