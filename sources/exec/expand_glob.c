/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_glob.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/13 12:28:06 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <dirent.h>
#include <stdio.h>
#include "minishell.h"
#include "errno.h"

// Return: 0) OK, 1) OOM, 2) EOF, 4) readdir error
static
uint8_t	stt_directory_read(DIR *dir_stream, const char *pattern, t_argv *arg)
{
	size_t			length;
	struct dirent	*dir_entry;
	char			*ptr;

	errno = 0;
	dir_entry = readdir(dir_stream);
	if (dir_entry == NULL)
	{
		if (errno != 0)
			perror("msh_readdir: ");
		return (((errno != 0) << 1) + 2);
	}
	if (ft_strwcmp(dir_entry->d_name, pattern) == 1)
	{
		length = ft_strlen(dir_entry->d_name) + 1;
		ptr = arg->data + arg->offset;
		if (ft_lmcpy(ptr, dir_entry->d_name, length, arg->end))
			return (1);
		arg->ptr[arg->count++] = ptr;
		arg->offset += length;
	}
	return (0);
}

// This function reads from a directory, compares against pattern with wildcard matching and 
// saves up to count entries inside the buffer supplied by arg
// This function does not null terminate
// Return: 0) OK, 1) OOM, 2) dir function problems, 4) exceeded count
uint8_t	expand_glob(const char *pattern, t_argv *arg, size_t count)
{
	DIR		*dir_stream;
	uint8_t	rvalue;

	dir_stream = opendir(".");
	if (dir_stream == NULL)
	{
		perror("msh_opendir: ");
		return (2);
	}
	rvalue = 0;
	while (rvalue == 0)
	{
		if (count == 0)
		{
			rvalue = 4;
			break ;
		}
		rvalue = stt_directory_read(dir_stream, pattern, arg);
		count--;
	}
	if (closedir(dir_stream) < 0)
		perror("msh_closedir: ");
	return (rvalue);
}
