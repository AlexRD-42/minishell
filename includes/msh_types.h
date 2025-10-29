/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:52:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/24 14:31:34 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_TYPES_H
# define MSH_TYPES_H

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

# ifndef FT_ENV_ENTRIES
#  define FT_ENV_ENTRIES 1024
# endif

# ifndef FT_ENV_SIZE
#  define FT_ENV_SIZE 65536
# endif

# ifndef FT_TOKEN_MAX
#  define FT_TOKEN_MAX 256
# endif

# define REDIR 0b000011110000000
//   (REDIR_IN | REDIR_OUT | APPEND | HEREDOC)

# define OPERATOR 0b0000000000011100
//   (OR | AND | PIPE)

typedef enum e_type
{
	ERROR = 1,
	UNSET = 0,
	WORD = 1 << 1,
	OR = 1 << 2,
	AND = 1 << 3,
	PIPE = 1 << 4,
	OPEN_PAREN = 1 << 5,
	CLOSE_PAREN = 1 << 6,
	REDIR_IN = 1 << 7,
	REDIR_OUT = 1 << 8,
	APPEND = 1 << 9,
	HEREDOC = 1 << 10,
	END = 1 << 11,
}	t_type;

typedef struct s_env
{
	size_t	count;
	size_t	offset;
	char	data[FT_ENV_SIZE];		// 64kb
	char	*ptr[FT_ENV_ENTRIES];	// 8kb
}	t_env;

typedef struct s_str
{
	union
	{
		char		*ptr;
		const char	*kptr;
	};
	size_t	length;
}	t_str;

typedef struct s_token
{
	const char		*str;
	uint16_t		type;
	uint16_t		length;
}	t_token;

typedef struct s_shell
{
	char	*input;
	t_env	env;
	t_token	tokens[FT_TOKEN_MAX];
	bool	mode; // 0 for non_interactive, 1 for interactive
}	t_shell;

#endif
