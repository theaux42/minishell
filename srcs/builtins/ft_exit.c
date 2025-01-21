/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:19:28 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/21 12:06:53 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exit_value(t_token *token)
{
	int	value;

	value = 0;
	if (token == NULL || token->value == NULL)
		return (0);
	if (ft_isdigits_str(token->value))
		value = ft_atoi(token->value);
	return (value % 256);
}

int	ft_exit(t_token *token, t_minishell *minishell, bool msg)
{
	int	status;

	status = exit_value(token);
	free_commands(minishell->commands);
	ft_free_builtins(minishell);
	if (msg)
		ft_printf("exit\n");
	exit(status);
	return (status);
}
