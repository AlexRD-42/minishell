/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:54:09 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/26 09:44:06 by adeimlin         ###   ########.fr       */
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

int64_t	ft_qmemcmp(const void *vptr1, const void *vptr2, size_t length)
{
	const char	*ptr1 = (const char *) vptr1;
	const char	*ptr2 = (const char *) vptr2;

	if ((uintptr_t) vptr1 == (uintptr_t) vptr2)
		return (0);
	while (length > sizeof(uintptr_t))
	{
		if (*ptr1 != *ptr2)
			return (1);
		if ((((uintptr_t)ptr1 | (uintptr_t)ptr2) & (sizeof(uintptr_t) - 1)) == 0)
			break ;
		length--;
	}
	while (length >= sizeof(uintptr_t))
	{
		if (*((const uintptr_t *)ptr1) != *((const uintptr_t *)ptr2))
			return (1);
		ptr1 += sizeof(uintptr_t);
		ptr2 += sizeof(uintptr_t);
		length -= sizeof(uintptr_t);
	}
	while (length > 0)
	{
		if (*ptr1 != *ptr2)
			return (1);
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
