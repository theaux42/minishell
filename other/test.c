/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 02:43:13 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/14 02:43:15 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>    // pour getenv
#include <sys/ioctl.h> // pour ioctl
#include <termios.h>   // pour les fonctions tcgetattr, tcsetattr
#include <unistd.h>    // pour isatty, ttyname

int	main(void)
{
	printf("%s\n", ttyname(0));
	printf("%s\n", ttyname(1));
	printf("%s\n", ttyname(2));
	printf("%i\n", isatty(0));
	printf("%i\n", ttyslot());
}
