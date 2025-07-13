/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:35:52 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/07 19:18:15 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>

static
void	*ft_memrcpy(void *dst_void, const void *src_void, size_t length)
{
	char		*dst;
	const char	*src = (const char *) src_void + length;

	if ((uintptr_t) dst_void == (uintptr_t) src_void)
		return (dst_void);
	dst = ((char *) dst_void) + length;
	while (length > sizeof(uintptr_t)
		&& (((uintptr_t)dst | (uintptr_t)src) & (sizeof(uintptr_t) - 1)))
	{
		*--dst = *--src;
		length--;
	}
	while (length >= sizeof(uintptr_t))
	{
		dst -= sizeof(uintptr_t);
		src -= sizeof(uintptr_t);
		length -= sizeof(uintptr_t);
		*((uintptr_t *)dst) = *((const uintptr_t *)src);
	}
	while (length > 0)
	{
		*--dst = *--src;
		length--;
	}
	return (dst_void);
}

void	*ft_memcpy(void *dst_void, const void *src_void, size_t length)
{
	char		*dst;
	const char	*src = (const char *) src_void;

	if ((uintptr_t) dst_void >= (uintptr_t) src_void)
		return (ft_memrcpy(dst_void, src_void, length));
	dst = (char *) dst_void;
	while (length > sizeof(uintptr_t)
		&& (((uintptr_t)dst | (uintptr_t)src) & (sizeof(uintptr_t) - 1)))
	{
		*dst++ = *src++;
		length--;
	}
	while (length >= sizeof(uintptr_t))
	{
		*((uintptr_t *)dst) = *((const uintptr_t *)src);
		dst += sizeof(uintptr_t);
		src += sizeof(uintptr_t);
		length -= sizeof(uintptr_t);
	}
	while (length > 0)
	{
		*dst++ = *src++;
		length--;
	}
	return (dst_void);
}

void	*ft_bzero(void *dst_void, size_t length)
{
	uint8_t	*dst;

	dst = (uint8_t *) dst_void;
	while (((uintptr_t)dst & (sizeof(uintptr_t) - 1)) && length > 0)
	{
		*dst++ = 0u;
		length--;
	}
	while (length >= sizeof(uintptr_t))
	{
		*((uintptr_t *)dst) = 0UL;
		dst += sizeof(uintptr_t);
		length -= sizeof(uintptr_t);
	}
	while (length > 0)
	{
		*dst++ = 0u;
		length--;
	}
	return (dst_void);
}

void	*ft_memset(void *dst_void, const uint8_t byte, size_t length)
{
	uint8_t			*dst;
	const uintptr_t	word_byte = byte * (0x0101010101010101 & UINTPTR_MAX);

	dst = (uint8_t *) dst_void;
	while (((uintptr_t)dst & (sizeof(uintptr_t) - 1)) && length > 0)
	{
		*dst++ = byte;
		length--;
	}
	while (length >= sizeof(uintptr_t))
	{
		*((uintptr_t *)dst) = word_byte;
		dst += sizeof(uintptr_t);
		length -= sizeof(uintptr_t);
	}
	while (length > 0)
	{
		*dst++ = byte;
		length--;
	}
	return (dst_void);
}
