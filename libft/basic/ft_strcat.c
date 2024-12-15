/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 22:30:22 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 14:07:59 by ededemog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**char	*ft_strcat(char *dest, char *src)
{
	int	size;
	int	i;

	size = ft_strlen(dest);
	i = 0;
	while (src[i] != '\0')
	{
		dest[size + i] = src[i];
		i++;
	}
	dest[size + i] = '\0';
	return (dest);
}**/

char *ft_strcat(char *dest, const char *src)
{
    size_t i;
    size_t j;

    i = 0;
    j = 0;
    if (!dest || !src)
        return (NULL);
    while (dest[i])
        i++;
    while (src[j])
    {
        dest[i + j] = src[j];
        j++;
    }
    dest[i + j] = '\0';
    return dest;
}
