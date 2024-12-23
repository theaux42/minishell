/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_edgecmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:08:47 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/23 06:41:32 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_edgecmp(const char *s1, const char s2)
{
	size_t	len;

	if (!s1)
		return (0);
	len = ft_strlen(s1);
	if (len > 0 && s1[0] == s2 && s1[len - 1] == s2)
		return (1);
	return (0);
}
