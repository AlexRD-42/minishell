/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:38:44 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/10 18:29:19 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>

size_t	ft_strlen(const char *str)
{
	const char	*ostr = str;

	while (*str != 0)
		str++;
	return ((size_t)(str - ostr));
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dst_size)
{
	const char	*osrc = src;

	while (*src != 0 && dst_size > 1)
	{
		*dst++ = *src++;
		dst_size--;
	}
	if (dst_size != 0)
		*dst = 0;
	while (*src != 0)
		src++;
	return ((size_t)(src - osrc));
}

size_t	ft_strlcat(char *dst, const char *src, size_t dst_size)
{
	char	*odst;

	odst = dst;
	while (dst_size > 0 && *dst != 0)
	{
		dst++;
		dst_size--;
	}
	while (*src != 0 && dst_size > 1)
	{
		*dst++ = *src++;
		dst_size--;
	}
	if (dst_size != 0)
		*dst = 0;
	while (*src++ != 0)
		dst++;
	return ((size_t)(dst - odst));
}

int32_t	ft_strncmp(const char *str1, const char *str2, size_t length)
{
	while (length > 0)
	{
		if (*str1 != *str2 || *str1 == 0)
			return (*(const uint8_t *) str1 - *(const uint8_t *) str2);
		str1++;
		str2++;
		length--;
	}
	return (0);
}

// wtf
int32_t	ft_strcmp(const char *str1, const char *str2)
{
	while (*str1 != 0 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return (*(const uint8_t *) str1 - *(const uint8_t *) str2);
}
