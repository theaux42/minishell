/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:27:21 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/24 14:38:27 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int command_length(t_prompt *prompt, int position)
{
    int length;
    int current_position;
    t_prompt *c;

    if (prompt == NULL)
        return (0);
    current_position = 0;
    c = prompt;
    length = 0;
    while (c && current_position != position)
    {
        if (c->type == PIPE)
            current_position++;
        c = c->next;
    }
    while (c && c->type != PIPE)
    {
        length++;
        c = c->next;
    }

    return (length);
}

