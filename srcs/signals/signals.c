/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:32:47 by ededemog          #+#    #+#             */
/*   Updated: 2025/01/31 23:54:45 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_signal_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	sig_event(void)
{
	return (EXIT_SUCCESS);
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("^C");
		rl_done = 1;
		g_signal = sig + 128;
	}
}

void	ft_signal(void)
{
	rl_event_hook = sig_event;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}
