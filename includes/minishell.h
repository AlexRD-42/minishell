/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:43:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/21 12:34:28 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdint.h>
# include <stddef.h>
# include <unistd.h>
# include <stdbool.h>

# ifndef FT_PAGE_SIZE
#  define FT_PAGE_SIZE 4096
# endif

# ifndef FT_PATH_MAX
#  define FT_PATH_MAX 4096
# endif

size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t dst_size);
size_t	ft_strlcat(char *dst, const char *src, size_t dst_size);
int32_t	ft_strncmp(const char *str1, const char *str2, size_t length);
int32_t	ft_strcmp(const char *str1, const char *str2);

void	*ft_memcpy(void *dst_void, const void *src_void, size_t length);
void	*ft_bzero(void *dst_void, size_t length);
void	*ft_memset(void *dst_void, const uint8_t byte, size_t length);

ssize_t	ft_write(int fd, const void *buffer, size_t length);

typedef struct s_shell
{
	char	*input;
	char	**envp;
	bool	non_interactive;
}	t_shell;

typedef struct s_str
{
	char	*ptr;
	size_t	length;
}	t_str;

typedef struct s_kstr
{
	const char	*ptr;
	size_t		length;
}	t_kstr;

#endif
