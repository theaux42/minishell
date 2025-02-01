/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 01:32:11 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/17 15:19:56 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar_printf(int c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

int	variable_manager(char c, va_list arguments, int fd)
{
	int	added;

	added = 0;
	if (c == 's')
		added += ft_putstr_printf(va_arg(arguments, char *), fd);
	else if (c == 'c')
		added += ft_putchar_printf((unsigned char)va_arg(arguments, int), fd);
	else if (c == 'x')
		added += ft_puthexa(va_arg(arguments, int), 0, fd);
	else if (c == 'X')
		added += ft_puthexa(va_arg(arguments, int), 1, fd);
	else if (c == '%')
		added += ft_putchar_printf('%', fd);
	else if (c == 'p')
		added += ft_putptr((unsigned long long)va_arg(arguments, void *), fd);
	else if (c == 'u')
		added += ft_putunbr(va_arg(arguments, unsigned int), fd);
	else if (c == 'i' || c == 'd')
		added += ft_putnbr(va_arg(arguments, int), fd);
	return (added);
}

int	output_manager(char *str, va_list arguments, int fd)
{
	int	i;
	int	printed;

	i = 0;
	printed = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '%')
		{
			i++;
			printed += variable_manager(str[i], arguments, fd);
		}
		else
		{
			ft_putchar_printf(str[i], fd);
			printed++;
		}
		i++;
	}
	return (printed);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	int		printed;
	va_list	args;

	if (fd < 0)
		return (-1);
	if (!format)
		return (-1);
	printed = 0;
	va_start(args, format);
	printed += output_manager((char *)format, args, fd);
	va_end(args);
	return (printed);
}

int	ft_printf(const char *format, ...)
{
	int		printed;
	va_list	args;

	if (!format)
		return (-1);
	printed = 0;
	va_start(args, format);
	printed += output_manager((char *)format, args, 1);
	va_end(args);
	return (printed);
}
