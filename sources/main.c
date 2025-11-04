/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 12:25:47 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/04 14:09:48 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_defines.h"

static inline
size_t	stt_line_end(const char *str, size_t length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		if (str[i] == 0 || str[i] == '\n')
			return (length);
		i++;
	}
	return (SIZE_MAX);
}

static
void	msh_begin(char *str, size_t length, t_shell *msh)
{

}

int	main(int argc, char **argv, char **envp)
{
	size_t	index;
	char	buffer[1024 * 1024];
	size_t	offset;
	ssize_t	bytes_read;
	t_shell	msh;

	offset = 0;
	bytes_read = read(STDIN_FILENO, buffer, FT_PAGE_SIZE);
	while (bytes_read > 0)
	{
		index = stt_line_end(buffer + offset, (size_t) bytes_read);
		if (index != SIZE_MAX)
		{
			if (buffer[offset + index] == 0)
				break ;
			if (buffer[offset + index] == '\n')
				msh_begin(buffer, offset + index, &msh);
		}
		offset += (size_t) bytes_read;
		bytes_read = read(STDIN_FILENO, buffer + offset, FT_PAGE_SIZE);
	}
	return (0);
}
