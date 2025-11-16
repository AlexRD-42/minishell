/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/16 14:06:04 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include "minishell.h"
#include "msh_types.h"

// Return: 0) EOF, 1) MATCH, 2) NO MATCH, -1) OOM, -2) readdir error
static
int	stt_directory_read(DIR *dir_stream, const char *pattern, t_argv *arg)
{
	size_t			length;
	struct dirent	*dir_entry;
	char			*ptr;

	errno = 0;
	dir_entry = readdir(dir_stream);
	if (dir_entry == NULL && errno == 0)
		return (0);
	else if (dir_entry == NULL)
	{
		perror("msh_readdir: ");
		return (-2);
	}
	if (ft_strwcmp(dir_entry->d_name, pattern) == 1)
	{
		length = ft_strlen(dir_entry->d_name) + 1;
		ptr = arg->data + arg->offset;
		if (ft_lmcpy(ptr, dir_entry->d_name, length, arg->end))
			return (-1);
		arg->ptr[arg->count++] = ptr;
		arg->offset += length;
		return (1);
	}
	return (2);
}

// This function reads from a directory, compares against pattern with wildcard matching and 
// saves up to count entries inside the buffer supplied by arg
// This function does not null terminate
// Return: 0) EOF, 1) At Least 1 Match -1) OOM, -2) dir function problems, -4) exceeded count
static
int	stt_expand_glob(const char *pattern, t_argv *arg, size_t count)
{
	DIR				*dir_stream;
	int				rvalue;
	const size_t	max_count = count;

	dir_stream = opendir(".");
	if (dir_stream == NULL)
	{
		perror("msh_opendir: ");
		return (-2);
	}
	rvalue = (arg->count + 1 >= FT_ARG_COUNT);
	rvalue = (rvalue == 0) - (rvalue << 2);
	while (rvalue > 0)
	{
		rvalue = stt_directory_read(dir_stream, pattern, arg);
		if ((count == 0 && rvalue == 1) || arg->count > (FT_ARG_COUNT - 1))
			rvalue = -4;
		count -= (rvalue == 1);
	}
	if (closedir(dir_stream) < 0)
		perror("msh_closedir: ");
	return (rvalue + (rvalue == 0 && max_count != count));
}

// Needs a better name
// Parses the string and performs variable expansion
// saving the results in a separate buffer supplied by argv
// Return: 0) OK, -1) OOM;
int	parse_interval(const char *str, const char *end, t_env *env, t_argv *arg)
{
	const char	*ostr = str;
	size_t		length;

	while (str < end)
	{
		if (*str == '$')
		{
			length = (size_t)(str - ostr);
			if (ft_lmcpy(arg->data + arg->offset, ostr, length, arg->end))
				return (-1);
			arg->offset += length;
			str = env_expand(str, end, arg, env);
			if (str == NULL)
				return (-1);
			ostr = str;
		}
		else
			str++;
	}
	length = (size_t)(str - ostr);
	if (ft_lmcpy(arg->data + arg->offset, ostr, length, arg->end))
		return (-1);
	arg->offset += length;
	return (0);
}

// Needs a better name
// Finds the interior length of the string between quotes, and copies it literally
// if its a single quote, or performs variable expansion if it isnt
// Return: NULL) OOM, !NULL) OK
static const
char	*stt_find_interval(const char *str, const char *end, t_env *env, t_argv *arg)
{
	const char	qtype = *str;
	const char	*ostr = str;
	size_t		length;

	if ((qtype == '"' || qtype == '\''))
	{
		ostr = ++str;
		while (str < end && *str != qtype)
			str++;
	}
	else
		while (str < end && *str != '"' && *str != '\'')
			str++;
	length = (size_t)(str - ostr);
	if (qtype == '\'')
	{
		if (ft_lmcpy(arg->data + arg->offset, ostr, length, arg->end))
			return (NULL);
		arg->offset += length;
	}
	else if (parse_interval(ostr, ostr + length, env, arg))
		return (NULL);
	return (str + (str < end));
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
	char		buffer[FT_ARG_SIZE];
	const char	*str = token->ptr;
	const char	*end = token->ptr + token->length;

	if (token->type & E_EXPAND)
		arg_ptr = &(t_argv){0, 0, buffer, NULL, buffer + sizeof(buffer)};
	else
		arg_ptr = arg;
	while (str < end)
	{
		str = stt_find_interval(str, end, env, arg_ptr);
		if (str == NULL)
			return (-1);
	}
	if (arg_ptr->data + arg_ptr->offset + 1 > arg_ptr->end)
		return (-1);
	arg_ptr->data[arg_ptr->offset++] = 0;
	if ((token->type & E_EXPAND))
		return (stt_expand_glob(buffer, arg, count));
	return (0);
}
