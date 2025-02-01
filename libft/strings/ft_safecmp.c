/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safecmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 06:25:33 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/30 06:26:12 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_safecmp(char *s1, char *s2)
{
	int	i;

	if (!s1 && !s2)
		return (-4242);
	if (!s1)
		return (-4242);
	if (!s2)
		return (-4242);
	i = 0;
	if (s1[0] == '\0' && s2[0] == '\0')
		return (0);
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
