/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:52:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/31 12:19:52 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_TYPES_H
# define MSH_TYPES_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "msh_defines.h"

typedef enum e_type
{
	UNSET = 0,
	ERROR = 1,
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

// Melhor manter como uint32_t, porque mantem o alinhamento de 16 bytes
// Caso seja necessario compactar, melhor usar token_small de 8 bytes
typedef struct s_token
{
	const char	*str;
	uint32_t	type;
	uint32_t	length;
}	t_token;

// Precisa do ponteiro inicial para manter referencia do offset. Ex:
// char *optr = posicao da str do primeiro token;
// depois pra usar o token, seria *(optr + offset);
typedef struct s_token_small
{
	uint32_t	offset;
	uint16_t	type;
	uint16_t	length;
}	t_token_small;

typedef struct s_shell
{
	char	*input;
	t_env	env;
	t_token	tokens[FT_TOKEN_MAX];
	bool	mode; // 0 for non_interactive, 1 for interactive
}	t_shell;

#endif
