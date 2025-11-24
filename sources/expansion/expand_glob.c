/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_glob.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 09:36:27 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/24 21:07:52 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "msh_types.h"
#include "msh_utils.h"

static
int32_t	stt_strwcmp(const uint8_t *str, const uint8_t *pattern)
{
	size_t			i;
	const uintptr_t	ostr = (uintptr_t) str - (*pattern == '*');

	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == (pattern[i] & 127))
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

// Return: 0) EOF, 1) MATCH, 2) NO MATCH, -1) OOM, -2) readdir error (P)
static
int	stt_directory_read(DIR *dir_stream, const char *pattern, t_vecp *vec)
{
	size_t			length;
	struct dirent	*dir_entry;

	errno = 0;
	dir_entry = readdir(dir_stream);
	if (dir_entry == NULL && errno == 0)
		return (0);
	else if (dir_entry == NULL)
		return (ft_error("msh_readdir: ", NULL, -2));
	if (stt_strwcmp((uint8_t *) dir_entry->d_name, (const uint8_t *) pattern))
	{
		length = ft_strlen(dir_entry->d_name) + 1;
		if (ft_lmcpy(vec->buf.wptr, dir_entry->d_name, length, vec->buf.end))
			return (-1);
		vec->ptr[vec->count++] = vec->buf.wptr;
		vec->buf.wptr += length;
		return (1);
	}
	return (2);
}

// This function reads from a directory, compares against pattern with wildcard
// matching and saves up to EOF entries inside the buffer supplied by arg.
// Return: >=0) Number of matches until EOF
// Return:	-1) OOM (P), -2) dir function problems (P), -4) exceeded count
ssize_t	msh_expand_glob(const char *pattern, t_vecp *vec)
{
	size_t	count;
	DIR		*dir_stream;
	ssize_t	rvalue;

	dir_stream = opendir(".");
	if (dir_stream == NULL)
		return (ft_error("msh_opendir: ", NULL, -2));
	count = 0;
	rvalue = (vec->count + 1 >= vec->max_count);
	rvalue = (rvalue == 0) - (rvalue << 2);
	while (rvalue > 0)
	{
		rvalue = stt_directory_read(dir_stream, pattern, vec);
		if (vec->count + 1 >= vec->max_count)
			rvalue = -4;
		count += (rvalue == 1);
	}
	if (closedir(dir_stream) < 0)
		ft_error("msh_closedir: ", NULL, 0);
	return (rvalue + (ssize_t)((rvalue == 0) * count));
}
