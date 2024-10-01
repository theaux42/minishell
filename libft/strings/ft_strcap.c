/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 20:04:28 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/30 18:07:04 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strcap(char *str)
{
	int		i;
	int		h;
	char	t;

	i = 0;
	h = -1;
	while (str[i] != '\0')
	{
		t = str[i];
		if (str[i] >= 65 && str[i] <= 90)
			str[i] = str[i] + 32;
		if ((str[h] < 'A' || str[h] > 'Z') && (str[h] < 'a' || str[h] > 'z' ))
			if (!(t < 'A' || t > 'z' || (t > 'Z' && t < 'a')))
				if ((str[h] < '0' || str[h] > '9'))
					str[i] = str[i] - 32;
		i++;
		h = i - 1;
	}
	return (str);
}
