/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:11:04 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/11 21:12:06 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoins(char **strings, char *separator)
{
	char	*result;
	char	*temp;
	int		i;

	i = 1;
	if (!strings)
		return (NULL);
	result = ft_strdup(strings[0]);
	while (strings[i])
	{
		temp = ft_strjoin(result, separator);
		if (!temp)
			return (NULL);
		result = ft_strjoin(temp, strings[i]);
		if (!result)
		{
			free(temp);
			return (NULL);
		}
		free(result);
		i++;
	}
	return (result);
}
