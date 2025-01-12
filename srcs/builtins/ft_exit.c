/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:19:28 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/11 07:11:22 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_token *token, t_minishell *minishell)
{
	(void)token;
	free_command(minishell->commands);
	ft_free_builtins(minishell);
	ft_printf("exit\n");
	exit(0);
}
