/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:33:51 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/14 09:29:51 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_printf(char *str, int fd)
{
	int	i;

	i = 0;
	if (!str)
		str = "(null)";
	while (str[i] != '\0')
	{
		ft_putchar_printf(str[i], fd);
		i++;
	}
	return (i);
}

void	ft_printptr(uintptr_t num, int fd)
{
	if (num >= 16)
	{
		ft_printptr(num / 16, fd);
		ft_printptr(num % 16, fd);
	}
	else
	{
		if (num <= 9)
			ft_putchar_printf((num + '0'), fd);
		else
			ft_putchar_printf((num - 10 + 'a'), fd);
	}
}

int	ft_putptr(unsigned long long ptr, int fd)
{
	int	print_length;

	print_length = 0;
	if (ptr == 0)
		print_length += ft_putstr_printf("(nil)", fd);
	else
	{
		print_length += ft_putstr_printf("0x", fd);
		ft_printptr(ptr, fd);
		print_length += ft_hexalen(ptr);
	}
	return (print_length);
}
