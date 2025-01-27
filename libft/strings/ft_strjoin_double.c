/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_double.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:11:19 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/26 12:07:32 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_double(char *s1, char *s2, char *s3, int amount_to_free)
{
	char	*temp;
	char	*result;

	if (!s1 || !s2 || !s3)
		return (NULL);
	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, s3);
	free(temp);
	if (!result)
		return (NULL);
	if (amount_to_free >= 1)
		free(s1);
	if (amount_to_free >= 2)
		free(s2);
	if (amount_to_free >= 3)
		free(s3);
	return (result);
}
