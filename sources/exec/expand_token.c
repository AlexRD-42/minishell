/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/13 14:38:22 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include "minishell.h"
#include "msh_types.h"

// Return: 0) OK, 1) OOM, 2) EOF, 4) readdir error
static
int	stt_directory_read(DIR *dir_stream, const char *pattern, t_argv *arg)
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
// Return: 0) OK, 1) OOM, 2) EOF, 4) dir function problems, 8) exceeded count
static
int	stt_expand_glob(const char *pattern, t_argv *arg, size_t count)
{
	DIR		*dir_stream;
	int		rvalue;

	dir_stream = opendir(".");
	if (dir_stream == NULL)
	{
		perror("msh_opendir: ");
		return (4);
	}
	rvalue = 0;
	while (rvalue == 0)
	{
		if (count == 0 || arg->count > (FT_ARG_MAX - 1))
		{
			rvalue = 8;
			break ;
		}
		rvalue = stt_directory_read(dir_stream, pattern, arg);
		count--;
	}
	if (closedir(dir_stream) < 0)
		perror("msh_closedir: ");
	return (rvalue);
}

static // Returns the interior length of the interval
size_t	stt_find_interval(const char *str, const char *end)
{
	const char	*ostr = str + (*str == '"' || *str == '\'');
	char		quote_type;

	if (ostr != str)
	{
		quote_type = *str++;
		while (*str != quote_type)
			str++;
	}
	else
	{
		while (str < end && *str != '"' && *str != '\'')
			str++;
	}
	return ((size_t)(str - ostr));
}

// Return: 0) OK, -1) OOM;
// To do: Fix bugs in the length calculation
static int
stt_parse_interval(const char *str, size_t length, t_env *env, t_argv *arg)
{
	const char	type = (*str == '\'') + ((*str == '"') << 1);
	const char	*end = str + length;
	const char	*ostr = str + (type != 0);

	str += (type == 2) + (type == 1) * length;
	while (str < end)
	{
		if (*str == '$')
		{
			length = (size_t)(str++ - ostr);
			if (ft_lmcpy(arg->data + arg->offset, ostr, length, arg->end))
				return (1);
			arg->offset += length;
			if (env_expand(&str, arg, env))
				return (1);
			ostr = str;
		}
		else
			str++;
	}
	length = (size_t)(str - ostr);
	if (ft_lmcpy(arg->data + arg->offset, ostr, length, arg->end))
		return (1);
	arg->offset += length;
	return (0);
}

// Return: 0) OK, -1) OOM, -2) dir function problems, -4) exceeded count;
/* This function expands a token, and globs the result if indicated by the token
The count variable serves as both a way of knowing if ptr is out of bounds, and
as a way of throwing errors for ambiguous redirects when expanding file names;
Saves the final results to arg, and uses a temporary arg_tmp buffer to store
the pattern if necessary*/
int	expand_token(t_token *token, t_env *env, t_argv *arg, size_t count)
{
	t_argv		*arg_ptr;
	char		buffer[FT_WCARD_SIZE];
	const char	*str = token->ptr;
	const char	*end = token->ptr + token->length;
	size_t		interval;

	if (token->type & E_EXPAND)
		arg_ptr = &(t_argv){0, 0, buffer, NULL, buffer + sizeof(buffer)};
	else
		arg_ptr = arg;
	while (str < end)
	{
		interval = stt_find_interval(str, end);
		if (stt_parse_interval(str, interval, env, arg_ptr))
			return (-1);
		str += interval;
	}
	if ((token->type & E_EXPAND) && arg->offset < sizeof(buffer))
	{
		buffer[arg->offset++] = 0;
		return (stt_expand_glob(buffer, arg, count));
	}
	return (0);
}
