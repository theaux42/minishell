/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 17:37:04 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/08 16:53:31 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Redirections:
// Since they are attached to the command, we create a new structure for them.
// This structure will contain the type of the redirection, the target file
// and a pointer to the next redirection.
// The parsing of it will be done in the get_redirections function.
// The function will create the structure and attach it to the command.
// and remove the redirection from the tokens list.
// Need to check if the redirection is well formed.
// There can't be two file redirections in a row.

t_redirection	*get_redirections(t_command *command, char *line)
{
	t_redirection	*head;
	t_redirection	*current;

	head = NULL;
}
