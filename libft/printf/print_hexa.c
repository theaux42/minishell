/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:43:05 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/14 09:43:42 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	ft_printhexa(int fd, unsigned int num, int isUpper)
{
	char	*charlist;

	if (isUpper)
		charlist = "0123456789ABCDEF";
	else
		charlist = "0123456789abcdef";
	if (num >= 16)
	{
		ft_printhexa(fd, num / 16, isUpper);
		ft_printhexa(fd, num % 16, isUpper);
	}
	else if (num <= 9)
		ft_putchar_printf(num + '0', fd);
	else
		ft_putchar_printf(charlist[num], fd);
}

int	ft_puthexa(unsigned int num, int isUpper, int fd)
{
	if (num == 0)
		return (write(fd, "0", 1));
	else
		ft_printhexa(fd, num, isUpper);
	return (ft_hexalen(num));
}
