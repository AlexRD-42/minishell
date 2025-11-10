/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:31:56 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/10 11:19:05 by adeimlin         ###   ########.fr       */
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

const
char	*ft_strchr(const char *str, unsigned char c)
{
	if (c == 0)
	{
		while (*str != 0)
			str++;
		return (str);
	}
	while (*str != 0)
	{
		if (*str == c)
			return (str);
		str++;
	}
	return (NULL);
}

// To do: add bounds checking
const
char	*ft_strfind(const char *str, const char *charset, uint8_t ref)
{
	size_t	i;
	size_t	lookup_table[256 / sizeof(size_t)];

	i = 0;
	while (i < (256 / sizeof(size_t)))
		lookup_table[i++] = 0UL;
	while (*charset != 0)
	{
		((uint8_t *)lookup_table)[(uint8_t)(*charset)] = 1;
		charset++;
	}
	ref = (ref != 0);
	((uint8_t *)lookup_table)[0] = ref;
	while (((uint8_t *)lookup_table)[(uint8_t)(*str)] != ref)
		str++;
	if (*str != 0)
		return (str);
	return (NULL);
}

int32_t	ft_strncmp(const char *str1, const char *str2, size_t length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		if (str1[i] != str2[i] || str1[i] == 0)
			return (((const uint8_t *) str1)[i] - ((const uint8_t *) str2)[i]);
		i++;
	}
	return (0);
}

// 0x00: NULL or past 127, 0x01: Info Chars
// 0x02: Space, 0x04 Symbols, 0x08: Digits
// 0x10: UPPER CASE, 0x20: lower case
// 0x40: _ + upper + lower + digits, 0x80: Hex
uint8_t	ft_ascii(const char c)
{
	static const uint8_t	ltable[256] = {
		0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
		0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
		0xC8, 0xC8, 0xC8, 0xC8, 0xC8, 0xC8, 0xC8, 0xC8,
		0xC8, 0xC8, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
		0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0x50, 0x50,
		0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
		0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
		0xD0, 0x50, 0x50, 0x04, 0x04, 0x04, 0x04, 0x44,
		0x04, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x60,
		0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
		0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0xE0,
		0x60, 0x60, 0x60, 0x04, 0x04, 0x04, 0x04, 0x01
	};

	return (ltable [(const uint8_t) c]);
}
