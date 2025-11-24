/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putstr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 17:59:57 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/24 18:00:51 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdint.h>

// Code duplication for itoa, so far unavoidable
// Check if write return is correct
ssize_t	ft_putnbr(int64_t number, int fd, char c)
{
	const int64_t	sign = (number >= 0) - (number < 0);
	char			buffer[32];
	char			*ptr;

	if (fd < 0)
		return (-1);
	ptr = buffer + 31;
	if (c != 0)
		*(--ptr) = c;
	*(--ptr) = sign * (number % 10) + '0';
	number = sign * (number / 10);
	while (number != 0)
	{
		*(--ptr) = (number % 10) + '0';
		number /= 10;
	}
	if (sign < 0)
		*(--ptr) = '-';
	return (write(fd, ptr, 32 - (uintptr_t)(ptr - buffer)));
}