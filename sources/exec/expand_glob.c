/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_glob.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/12 15:26:39 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <dirent.h>
#include <stdio.h>
#include "minishell.h"
#include "msh_defines.h"
#include "errno.h"

// Need to store: 
// pattern string created from token
// directory string containing file names
// final result
// char	*expand_dir(const char *str, size_t length, t_env *env, char *buffer)
static
DIR	*stt_directory_init(void)
{
	char	path[FT_PATH_MAX];
	DIR		*dir_stream;

	if (getcwd(path, FT_PATH_MAX) == NULL)
	{
		perror("msh_getcwd: ");
		return (NULL);
	}
	dir_stream = opendir(path);
	if (dir_stream == NULL)
		perror("msh_opendir: ");
	return (dir_stream);
}

// 0: Valid Entry, 1: End of read, 2: readdir error, 4: out of memory error
static
uint8_t	stt_directory_read(DIR *dir_stream, const char *pattern, t_argv *arg)
{
	size_t			length;
	struct dirent	*dir_entry;

	errno = 0;
	dir_entry = readdir(dir_stream);
	if (dir_entry == NULL)
	{
		if (errno != 0)
			perror("msh_readdir: ");
		return ((errno != 0) << 1);
	}
	if (ft_strwcmp(dir_entry->d_name, pattern) == 1)
	{
		length = ft_strlen(dir_entry->d_name) + 1;
		if (arg->offset + length > FT_WCARD_SIZE)
		{
			ft_write(2, "msh_stack: out of memory\n", 26);
			return (4);
		}
		arg->ptr[arg->count++] = ft_memcpy(arg->data + arg->offset, dir_entry->d_name, length);
		arg->offset += length;
	}
	return (1);
}

// This function reads from a directory, compares against pattern with wildcard matching and 
// saves up to count entries inside the buffer supplied by arg
// 0: No Problems, 1: dir function problems, 2: exceeded count, 4: exceeded limit
// This function does not null terminate
uint8_t	expand_glob(const char *pattern, t_argv *arg, size_t count)
{
	DIR		*dir_stream;
	size_t	i;
	uint8_t	rvalue;

	dir_stream = stt_directory_init();
	if (dir_stream == NULL)
		return (1);
	i = 1;
	rvalue = stt_directory_read(dir_stream, pattern, arg);
	while (rvalue == 0)
	{
		if (i > count)
		{
			rvalue = 2;
			break ;
		}
		rvalue = stt_directory_read(dir_stream, pattern, arg);
		i++;
	}
	if (closedir(dir_stream) < 0)
		perror("msh_closedir: ");
	return (rvalue & 1);
}
