/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_cmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 14:07:38 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/11 14:44:04 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>

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

int32_t	ft_strwcmp(const char *str, const char *pattern)
{
	size_t			i;
	const uintptr_t	ostr = (uintptr_t) str - (*pattern == '*');

	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == pattern[i])
			i++;
		else if (pattern[i] == '*')
		{
			pattern += i + 1;
			str += i;
			i = 0;
		}
		else if ((uintptr_t) str != ostr)
		{
			i = 0;
			str++;
		}
		else
			return (0);
	}
	while (pattern[i] == '*')
		i++;
	return (pattern[i] == 0);
}
