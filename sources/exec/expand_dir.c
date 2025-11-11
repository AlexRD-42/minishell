/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/11 20:37:30 by adeimlin         ###   ########.fr       */
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
	return (dir_stream);
}

static size_t
stt_directory_loop(DIR *dir_stream, const char *pattern, char **argv, char *buffer)
{
	size_t			i;
	size_t			length;
	struct dirent	*dir_entry;

	i = 0;
	errno = 0;
	dir_entry = readdir(dir_stream);
	while (dir_entry != NULL)
	{
		if (ft_strwcmp(dir_entry->d_name, pattern) == 1)
		{
			length = ft_strlen(dir_entry->d_name) + 1;
			argv[i++] = ft_memcpy(buffer, dir_entry->d_name, length);
			buffer += length;
		}
		dir_entry = readdir(dir_stream);
	}
	if (dir_entry == NULL && errno != 0)
		perror("msh_readdir: ");
	return (i);
}

size_t	expand_dir(const char *pattern, char **argv, char *buffer)
{
	DIR		*dir_stream;
	size_t	i;

	dir_stream = stt_directory_init();
	if (dir_stream == NULL)
	{
		perror("msh_opendir: ");
		return (0);
	}
	i = stt_directory_loop(dir_stream, pattern, argv, buffer);
	if (closedir(dir_stream) < 0)
		perror("msh_closedir: ");
	return (i);
}
