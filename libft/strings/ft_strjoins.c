/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:11:04 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 06:01:49 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoins(char **strings, int count)
{
	char	*result;
	char	*temp;
	int		i;

	if (!strings || count <= 0)
		return (NULL);
	result = ft_strdup(strings[0]);
	if (!result)
		return (NULL);
	i = 1;
	while (i < count && strings[i])
	{
		temp = result;
		result = ft_strjoin(temp, strings[i]);
		free(temp);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}
