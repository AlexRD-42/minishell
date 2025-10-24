/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_find.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:54:09 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/24 17:13:53 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>

// Needs optimization
const
void	*ft_memchr(const void *vptr, uint8_t byte, size_t length)
{
	const uint8_t	*ptr = (const uint8_t *) vptr;

	while (length > 0)
	{
		if (*ptr == byte)
			return (ptr);
		length--;
		ptr++;
	}
	return (NULL);
}

// Needs optimization
int64_t	ft_memcmp(const void *vptr1, const void *vptr2, size_t length)
{
	const uint8_t	*ptr1 = (const uint8_t *) vptr1;
	const uint8_t	*ptr2 = (const uint8_t *) vptr2;

	if (vptr1 == vptr2)
		return (0);
	while (length > 0)
	{
		if (*ptr1 != *ptr2)
			return (*ptr1 - *ptr2);
		ptr1++;
		ptr2++;
		length--;
	}
	return (0);
}

int64_t	ft_memrcmp(const void *vptr1, const void *vptr2, size_t length)
{
	const uint8_t	*ptr1 = (const uint8_t *) vptr1 + length - 1;
	const uint8_t	*ptr2 = (const uint8_t *) vptr2 + length - 1;

	if (vptr1 == vptr2)
		return (0);
	while (length > 0)
	{
		if (*ptr1 != *ptr2)
			return (*ptr1 - *ptr2);
		ptr1--;
		ptr2--;
		length--;
	}
	return (0);
}
